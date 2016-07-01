<?php 

set_time_limit( 3600 );
ini_set("memory_limit", "100000M");

require_once "config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once "_funcoes.php";
require_once "_constantes.php";

$db = new cls_banco();

ob_clean();

require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if( $db->testa_superuser() ){
	
	$sql = "SELECT
			dop.dopid,
			mdoqtdvalidacao,
			count(dpvid) as qtd
		FROM
			par.documentopar dop
		INNER JOIN par.modelosdocumentos 	mdo ON mdo.mdoid = dop.mdoid
		INNER JOIN par.documentoparvalidacao 	dpv ON dpv.dopid = dop.dopid
		WHERE arqid_documento IS NULL
		GROUP BY dop.dopid, mdoqtdvalidacao
		HAVING mdoqtdvalidacao >= count(dpvid)";
	
	$documentos = $db->carregar( $sql );
	$documentos = is_array($documentos) ? $documentos : Array();
	
	foreach( $documentos as $documento ){
		
		ob_clean();
		
		require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
				
		$html = pegaTermoCompromissoArquivo( $documento['dopid'], '' );
		
		$sql = "SELECT
							dpv.dpvcpf as cpfgestor,
							to_char(dpvdatavalidacao, 'DD/MM/YYYY HH24:MI:SS') as data,
							us.usunome,
							us.usucpf
						FROM par.documentopar  dp
						INNER JOIN par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid
						INNER JOIN seguranca.usuario us ON us.usucpf = dpv.dpvcpf
						WHERE
							dpv.dpvstatus = 'A' and
							dp.dopid = ".$documento['dopid'];
		
		$dadosValidacao = $db->carregar($sql);
		
		$textoValidacao = "";
		
		if(is_array($dadosValidacao)){
			foreach( $dadosValidacao as $dv ){
				$cpfvalidacao[] = $dv['cpfgestor'];
				$textoValidacao .= "<b>Validado por ".$dv['usunome']." - CPF: ".formatar_cpf($dv['usucpf'])." em ".$dv['data']." </b><br>";
			}
		}
		
		$html = trim(html_entity_decode( $html ))."
				<table id=termo align=center border=0 cellpadding=3 cellspacing=1 >
					<tr style=\"text-align: center;\">
						<td><b>VALIDAÇÃO ELETRÔNICA DO DOCUMENTO<b><br><br>$textoValidacao</td>
					</tr>
				</table>";
		
		$html = str_replace("'", '', $html);
		
		$pdf = pdf( utf8_encode( $html ) );
		
		$descricaoArquivo = 'PAR'.str_pad($documento['dopid'],8,0, STR_PAD_LEFT).'/'.date('d-m-Y');
		
		$campos = Array();
		
		$file = New FilesSimec("documentopar", $campos, 'par');
		
		$file->arquivo['name'] = $descricaoArquivo.".pdf";
		
		$arquivoSalvo = $file->setStream( $descricaoArquivo, $pdf, "text/pdf", ".pdf", false);
		
		$sql = "UPDATE par.documentopar SET
					arqid_documento = $arquivoSalvo
				WHERE
					dopid = {$documento['dopid']}";
		
		$db->executar($sql);
		
		$db->commit();
		
		unset($file);
		unset($arquivoSalvo);
		unset($descricaoArquivo);
		unset($pdf);
		unset($html);
		unset($dadosValidacao);
	}
}
?>