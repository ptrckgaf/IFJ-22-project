<?php
declare(strict_types=1);

function bar(string $param) : string { return foo($param);
}

function foo(string $param) : string {
return bar($param); }




