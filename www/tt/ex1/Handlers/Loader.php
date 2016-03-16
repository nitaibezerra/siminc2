<?php
/**
 * This simple Part is taking responsibility of Loading Other Class without path problems
 * @package Loading_Handlers
 * @author Mohammed Yousef Bassyouni <harrrrpo@gmail.com>
 * @license http://opensource.org/licenses/gpl-license.php GNU Public License
 */
/**
 * This is a very simple function that given a path go $n levels higher
 * IT MUST BE INCLUDED ON EVERY FILE USING THIS CLASS (PHPThreader)
 * it's used for adjusting relatve locations
 *
 * @param string $path
 * @param string $n
 * @return string
 */
function Up($path,$n)
{
	for ($i=0;$i<$n;$i++)
	$path=dirname($path);
	return $path."/";
}
?>