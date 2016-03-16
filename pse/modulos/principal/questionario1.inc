<?php
include_once( APPRAIZ. "includes/classes/modelo/spm/Tabela.class.inc" );
include_once( APPRAIZ. "includes/classes/modelo/spm/Tabelaresposta.class.inc" );
include_once( APPRAIZ. "includes/classes/modelo/spm/Grupocoluna.class.inc" );
include_once( APPRAIZ. "includes/classes/modelo/spm/Grupolinha.class.inc" );
include_once( APPRAIZ. "includes/classes/modelo/spm/Linha.class.inc" );
include_once( APPRAIZ. "includes/classes/modelo/spm/Coluna.class.inc" );
include_once( APPRAIZ. "includes/classes/modelo/spm/Montatabela.class.inc" );

header('Content-type: text/html; charset="iso-8859-1"',true);
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre

$perid = 136;
$qrpid = 21;

$obMontatabela = new Montatabela( );

if( $_POST ){
	
	$obTabelaResposta = new Tabelaresposta( );
	$obTabelaResposta->excluirPorQuestionarioTabela( $qrpid, $_POST['idTabela'] );
	$obTabelaResposta->commit();
	
	foreach ($_POST as $chave => $values)
	{
		if($values <> ''){
			if(strstr($chave,"campo_"))
			{	
				$obTabelaResposta = new Tabelaresposta( );
				list( , $linha, $coluna ) = explode("_",$chave);;
				
				$arrCampos = array('tblid', 'linid', 'colid', 'qrpid', 'tbrdsc');
				$arrDados  = array("tblid" => $_POST['idTabela'],
									  "linid" => $linha,
									  "colid" => $coluna,
									  "qrpid" => $qrpid,
									  "tbrdsc" => $values);
				$obTabelaResposta->popularObjeto( $arrCampos, $arrDados );
				$obTabelaResposta->salvar();
				$obTabelaResposta->commit();
							
			}
		}
	}
	die("<SCRIPT>alert('Operação realizada com sucesso.'); window.location.replace( window.location );</SCRIPT>");
	
}

include  APPRAIZ."includes/cabecalho.inc";
?>

<script type="text/javascript">
</script>


<?

$obMontatabela->montaNovaTabela( $perid, $qrpid );

?>