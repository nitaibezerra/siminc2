<?php
	/**
	 * Mostra o conteúdo do determinado passado
	 *
	 * Funcionalidade de grande importância para o sistema, o debug mostra os dados de uma variável passada,
	 * seja ela string, inteiro, objeto, array, etc.
	 *
	 * @param mixed $mixValor Recebe a variável a mostrar o conteúdo
	 * @param boolean $boolDie Define se após mostrar o conteúdo da variável interromperá o script atual
	 */
	function debug( $mixValor , $boolDie = true) {
		$debug = debug_backtrace();
		$arrDebug = $debug[0];
		
		echo "<fieldset style='border:1px solid #000;font-family:verdana; font-size:11px'>";
		echo "<legend style='font-family:verdana;font-size:11px;font-weight:bold'>DEBUG</legend>";
		
		foreach($arrDebug as $strKey => $strValue) {
			echo "<b>" . $strKey . ": </b>" . $strValue . "<br />";
		}
		
		echo "<hr noshade='true' />";
		//if( is_array( $mixValor ) || is_object( $mixValor ) )
		//{
			//highlight_string( $mixValor );
			echo nl2br(highlight_string(print_r($mixValor, true), true));
		//}
		//else
		//{
		//	echo var_dump( $mixValor) ;
		//}
		echo "	</fieldset>";
		
		if ($boolDie == TRUE) {
			echo "<div style='font-size:14px;font-weight:bold;color:#CC0000;font-family:arial;'>DIE</div>";
			exit();
		}
	}
?>