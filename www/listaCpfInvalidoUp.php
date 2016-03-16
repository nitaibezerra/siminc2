<?php
if (isset($_REQUEST['invalidos'])) {
    echo '.<br />';
    echo exec('svn up /var/www/simec/simec_dev/simec');
}
