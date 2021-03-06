local(here) = currentCapture->callSite_file->stripLastComponent + '/'
not #here->beginsWith('/') or #here->size == 1
    ? #here = io_file_getcwd + '/' + #here
if(not var_defined('_prep4tests')) => {
    sourcefile(file(#here + './spec_helper.inc'), -autoCollect=false)->invoke
}


describe(::http_request) => {
    it(`Takes in a URL string and parses out all the parts correctly`) => {
        local(req) = http_request(`http://example.com/foo/bar`)

        expect(`http://example.com/foo/bar`, #req->url)
        expect(`http://`       , #req->urlProtocol)
        expect(`example.com`   , #req->urlHostname)
        expect(`/foo/bar`      , #req->urlPath)
    }

    it(`Properly parses https:// URLs`) => {
        local(req) = http_request(`https://example.com/foo/bar`)

        expect(`https://example.com/foo/bar`, #req->url)
        expect(`https://`       , #req->urlProtocol)
        expect(`example.com`   , #req->urlHostname)
        expect(`/foo/bar`      , #req->urlPath)
    }

    it(`Allows for not specifying http:// for non-secure requests`) => {
        local(req) = http_request(`example.com/foo/bar`)

        expect(`http://example.com/foo/bar`, #req->url)
        expect(`http://`       , #req->urlProtocol)
        expect(`example.com`   , #req->urlHostname)
        expect(`/foo/bar`      , #req->urlPath)
    }

    it(`Takes in a URL string and parses out all the parts correctly including query params`) => {
        local(req) = http_request(`http://example.com/foo/bar?baz=2`)

        expect(`http://example.com/foo/bar?baz=2`, #req->url)
        expect(`http://`       , #req->urlProtocol)
        expect(`example.com`   , #req->urlHostname)
        expect(`/foo/bar`      , #req->urlPath)
        expect(pair(`baz`, `2`), #req->getParams->first)
    }

    it(`Properly handles GET parameters in both the URL and passed as an argument`) => {
        local(req) = http_request(
            `https://example.com/run?rhinos=1&bulls=0`,
            -getParams = (:`hippos` = 2)
        )

        expect(
            (:`rhinos`=`1`,`bulls`=`0`, `hippos`=2),
            #req->getParams
        )
    }
}