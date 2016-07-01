<?
if($_REQUEST['requisicao'] == 'enviarArquivo') {

	$dados = file($_FILES['arquivo']['tmp_name']);

	foreach($dados as $numline => $line) {
		$columns = explode("|", $line);
								
		if( $numline > 0 )
		{
//			$a = $columns[1];
//			ver( mb_detect_encoding( $a ) );
//			
//			$b = mb_convert_encoding( $a , 'ISO-8859-1', 'UTF-8');
//			ver( mb_detect_encoding( $b , "auto") );
//			die();
			
			//tratando cidades
			$cidadeOrigem = $columns[2];
			$cidadeDestino = $columns[3];

			$cidadeOrigem = ereg_replace("[ÁÀÂÃ]","A",$cidadeOrigem);
			$cidadeOrigem = ereg_replace("[áàâãª]","a",$cidadeOrigem);
			$cidadeOrigem = ereg_replace("[ÉÈÊ]","E",$cidadeOrigem);
			$cidadeOrigem = ereg_replace("[éèê]","e",$cidadeOrigem);
			$cidadeOrigem = ereg_replace("[ÓÒÔÕ]","O",$cidadeOrigem);
			$cidadeOrigem = ereg_replace("[óòôõº]","o",$cidadeOrigem);
			$cidadeOrigem = ereg_replace("[ÚÙÛ]","U",$cidadeOrigem);
			$cidadeOrigem = ereg_replace("[úùû]","u",$cidadeOrigem);
			$cidadeOrigem = str_replace("Ç","C",$cidadeOrigem);
			
			$cidadeDestino = ereg_replace("[ÁÀÂÃ]","A",$cidadeDestino);
			$cidadeDestino = ereg_replace("[áàâãª]","a",$cidadeDestino);
			$cidadeDestino = ereg_replace("[ÉÈÊ]","E",$cidadeDestino);
			$cidadeDestino = ereg_replace("[éèê]","e",$cidadeDestino);
			$cidadeDestino = ereg_replace("[ÓÒÔÕ]","O",$cidadeDestino);
			$cidadeDestino = ereg_replace("[óòôõº]","o",$cidadeDestino);
			$cidadeDestino = ereg_replace("[ÚÙÛ]","U",$cidadeDestino);
			$cidadeDestino = ereg_replace("[úùû]","u",$cidadeDestino);
			$cidadeDestino = str_replace("Ç","C",$cidadeDestino);
			
			//tratando campos do tipo ponto flutuante
			$columns[5] = str_replace(",",".",$columns[5]);
			$columns[8] = str_replace(",",".",$columns[8]);			
			$columns[9] = str_replace(",",".",$columns[9]);
			
			//formatando data			
			$data_formatada = $columns[6][6] . $columns[6][7] . $columns[6][8] . $columns[6][9] . $columns[6][3] . $columns[6][4] . $columns[6][0] . $columns[6][1];
			
			// achando os aeroportos
			$sql_aero_origem =
					"SELECT * 
					FROM evento.paaeroporto
					WHERE 
					upper(removeacento( AERCIDADE )) like upper(removeacento(' ".$cidadeOrigem."'))
					LIMIT 1";
				
			$sql_aero_destino =
					"SELECT * 
					FROM evento.paaeroporto
					WHERE 
					upper(removeacento( AERCIDADE )) like upper(removeacento(' ".$cidadeDestino."'))
					LIMIT 1";

			$cidadeDestino = $db->pegaUm( $sql_aero_destino );
			$cidadeOrigem = $db->pegaUm( $sql_aero_origem );
				
			$sql_passagem_scdp =
			"INSERT INTO evento.PAPASSAGEM_SCDP
				( 
					AERIDORI ,
					AERIDDES ,
					PSDNUMPCDP ,
					PSDNOPROPOSTO ,
					PSDNUVOO ,
					PSDTARIFAPRATICADA ,
					PSDDTEMISSAOBILHETE ,
					PSDNUBILHETE ,
					";
			
			if ( $columns[8] !== '' and $columns[8] !== '0'  )
			{
				$sql_passagem_scdp .= "PSDVLRMULTA ,";
			}

			if ( $columns[9] !== '' and $columns[9] !== '0'  )
			{
				$sql_passagem_scdp .= "PSDVLRDIFREMARCACAO ,";
			}
			
			$sql_passagem_scdp .=
			"PSDANOEMPENHO ,
					PSDNUEMPENHO ,
					PSDDSEMPENHO ,
					PSDNOORGAO
				) 
			VALUES
				(
					$cidadeOrigem ,
					$cidadeDestino ,
					'". mb_convert_encoding( $columns[0] , 'ISO-8859-1', 'UTF-8' ) ."' ,
					'". $columns[1] ."' ,
					'". mb_convert_encoding( $columns[4] , 'ISO-8859-1', 'UTF-8' ) ."' ,
					'". $columns[5] ."' ,
					'". $data_formatada ."' ,
					'". utf8_decode( $columns[7] ) ."' ,";
			if ( $columns[8] !== '' and $columns[8] !== '0'  )
			{
				$sql_passagem_scdp .= " '$columns[8]' ,";
			}
			
			if ( $columns[9] !== '' and $columns[9] !== '0'  )
			{
				$sql_passagem_scdp .= " '$columns[9]' ,";
			}
			
			$sql_passagem_scdp .=
			"
					$columns[10] ,
					$columns[11] ,
					'". mb_convert_encoding( $columns[12] , 'ISO-8859-1', 'UTF-8' ) ."' ,
					'". mb_convert_encoding( $columns[13] , 'ISO-8859-1', 'UTF-8' ) ."'				
				);";

			$sql_verifacao = 
			"SELECT * FROM evento.PAPASSAGEM_SCDP WHERE PSDNUMPCDP like '" . mb_convert_encoding( $columns[0] , 'ISO-8859-1', 'UTF-8' ) ."'";
			
			$existeocara = $db->pegaUm( $sql_verifacao );
			
			if ( !$existeocara )
			{
				$db->executar( $sql_passagem_scdp );
			}		
				
		}
	}
	if($db->commit()){
		echo "<script>
			alert('Operação Realizada com Sucesso!');
			window.location.href = 'passagens.php?modulo=principal/cargascdp&acao=I';
			 </script>";
		exit();
	} else {
		$db->rollback();
		echo "<script>
				alert('Falha na Operação');
				window.location.href = 'passagens.php?modulo=principal/cargascdp&acao=I';
			 </script>";
		exit();
	}
}


include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

$db->cria_aba($abacod_tela,$url,'');
//cte_montaTitulo( 'Carga de dados Financeiros', '&nbsp;' );

?>
<script>
function validarEnvioCarga() {

	if(document.getElementById('arquivocsv').value == '') {
		alert('Selecione um arquivo para enviar');
		return false;
	}
		
	document.getElementById('formulario').submit();
	
}
</script>
<form method="post" name="formulario" id="formulario"
	enctype="multipart/form-data" action="" target="iframeUpload"><input
	type="hidden" name="requisicao" value="enviarArquivo">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloCentro" colspan="2">Passagens - Carga de Planilha
		SCDP</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Arquivo:</td>
		<td><input type="file" name="arquivo" id="arquivocsv"></td>
	</tr>
	<tr style="background-color: #DCDCDC">
		<td align="center" colspan="2"><input type="button" value="Enviar"
			onclick="validarEnvioCarga();"></td>
	</tr>
</table>
</form>
