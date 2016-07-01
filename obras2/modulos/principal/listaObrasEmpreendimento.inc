<?php 
// empreendimento || obra || orgao
verificaSessao( 'empreendimento' );

switch ( $_REQUEST['req'] ){
	case 'abaFotos':
		$_SESSION['imgparams'] = array("filtro" => "obrid = ".$_GET['obrid'], 
									   "tabela" => "obras2.obras_arquivos");
		header('Location: /slideshow/slideshow/index.php?pagina=&_sisarquivo=obras2&arqid=' . $_GET['arqid']);
		die();
	case 'abaDocumento':
		$_SESSION['obras2']['obrid'] = $_POST['obrid'];
		$obrid = $_SESSION['obras2']['obrid'];
		$obras = new Obras( $obrid );
		$_SESSION['obras2']['empid'] = $obras->empid;
		header('Location: ?modulo=principal/cadObraDocumentos&acao=A');
		die();
	case 'abaRestricao':
		$_SESSION['obras2']['obrid'] = $_POST['obrid'];
		$obrid = $_SESSION['obras2']['obrid'];
		$obras = new Obras( $obrid );
		$_SESSION['obras2']['empid'] = $obras->empid;
		header('Location: ?modulo=principal/listaRestricao&acao=O');
		die();
}



function apagar(){
	global $db;
	
	$obra = new Obras( $_REQUEST['obrid'] );
	if ( $obra->obrid ){
		$obra->obrstatus = 'I';
		$obra->salvar();
	}
	$db->commit();
	die('<script type="text/javascript">
			alert(\'Operação realizada com sucesso!\');
			location.href=\'?modulo=principal/listaObrasEmpreendimento&acao=A\';
		 </script>');
}

function pegaHtmlPagamento(){
	global $db;
	
	if ($_REQUEST['preid']) {
		/* Substituido dia 23/12/2013 parnumseqob ->   pagnumeroob
		 * Analista: Daniel Areas Programador Eduardo Duniceu
		* */
		$sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento 
			FROM par.empenhoobra emo 
			INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A' 
			INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
			INNER JOIN par.pagamento pag ON pag.empid = emo.empid   
			WHERE emo.preid = '".$_REQUEST['preid']."' AND pag.pagstatus='A'
			UNION ALL
			SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento 
			FROM par.empenhoobrapar emo 
			INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A' 
			INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
			INNER JOIN par.pagamento pag ON pag.empid = emo.empid   
			WHERE emo.preid = '".$_REQUEST['preid']."' AND pag.pagstatus='A'";
		
		$pagamentoobra = $db->pegaLinha($sql);
		
		if($pagamentoobra) {
			echo "Pago<br>
				  Valor pagamento(R$): ".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."<br> 
				  Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob']."<br> 
				  Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
				  Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
				  ";	
		} else {
			echo "Não pago";	
		}
		
	}	
	die();
}


if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	die();
}

$_SESSION['obras2']['obrid'] = '';


$empid = $_REQUEST['empid'] ? $_REQUEST['empid'] : $_SESSION['obras2']['empid'];
$_SESSION['obras2']['empid'] = $empid;


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
cria_abas_Obras(ID_ABA_EMP_CADASTRADO,$url,$parametros);

$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );
// ver($empreendimento,d);
$empreendimento->montaCabecalho();

monta_titulo( 'Lista de Obras do Empreendimento', '' );

//$empreendimento = new Empreendimento( $_SESSION['obras2']['empid'] );

?>

<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
var u='?modulo=principal/listaObrasEmpreendimento&acao=A&req=pegaHtmlPagamento&preid=';

$(document).ready(function(){

	$('.pesquisar').click(function(){
		$('#formListaObra').submit();
	});

	$('.exibeBusca').click(function(){
		if( $('#tr_busca_avancada').css('display') == 'none' ){
			$('#tr_busca_avancada').show();
		}else{
			$('#tr_busca_avancada').hide();
		}
	});

	$('.novoObjeto').click(function(){
		window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A';
	});
});

function abreEvolucaoFinan( obrid ){
	janela('?modulo=principal/grafico_evolucao_financeira&acao=A&obrid='+obrid ,800,650);
}

function alterarObr( obrid ){
	location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function novaObrVinculada( obrid ){
	location.href = '?modulo=principal/cadObra&acao=A&obridVinculado=' + obrid;
}

function excluirObr( obrid ){
	if ( confirm('Deseja apagar esta obra?') ){
		$('#obrid').val(obrid);
		$('#req').val('apagar');
		$('#formListaObra').submit();
	}
}

function abreDocumentoObjeto( obrid ){
	$('#req').val('abaDocumento');
	$('#obrid').val(obrid);
	
	$('#formListaObra').submit();
}

function abreRestricao( obrid ){
	$('#req').val('abaRestricao');
	$('#obrid').val(obrid);
	
	$('#formListaObra').submit();
}

function abreFotoObjeto( obrid, arqid ){
	janela('?modulo=principal/listaObrasEmpreendimento&acao=A&req=abaFotos&obrid=' + obrid + '&arqid=' + arqid, 800, 650);
}
</script>
<form method="post" name="formListaObra" id="formListaObra">
	<input type="hidden" name="req" id="req" value=""/>
	<input type="hidden" name="obrid" id="obrid" value=""/>
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
			<td>
				<?=campo_texto('obrbuscatexto','N','S','',70,100,'','', '', '', '', 'id="obrbuscatexto"', '', $_REQUEST['obrbuscatexto']);?>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<a class="exibeBusca" style="cursor:pointer">[Busca avançada]</a>
			</td>
		</tr>
		<tr>
			<td id="tr_busca_avancada" colspan="2" style="display:none">
				<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
					<tr>
						<th colspan="2">Busca Avançada</th>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Tipo de Obra:</td>
						<td>
							<?php 
							$tipoObra = new TipoObra();
							$db->monta_combo( "tobid", $tipoObra->listaCombo(), "S", "Todos", "", "", "", "", "N", "tobid", '', $_POST['tobid'] );
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Classificação da Obra:</td>
						<td>
							<?php 
							$classificacaoObra = new ClassificacaoObra();				
							$db->monta_combo( "cloid", $classificacaoObra->listaCombo(), "S", "Todos", "", "", "", "", "N", "cloid", '', $_POST['cloid'] );
							?>
						</td>
					</tr>
				</table>			
			</td>
		</tr>
		<tr>
			<td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
				<?php 
				$esdid = pegaEstadoEmpreendimento( $empreendimento->docid );
				if ( $esdid == ESDID_EMP_DIVISAO ):
				?>
				<div style="float:left" class="novoObjeto" > 
					<a style="cursor: pointer;"> <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Nova Obra"> Nova Obra</a>
				</div>
				<?php 
				endif;
				?>
				<input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
			</td>
		</tr>
	</table>
</form>
<?php 
$obras 					 = new Obras();
$filtro 				 = $_POST;
$filtro['empid'] 		 = $empreendimento->empid;
$filtro['not(obridpai)'] = true;
$sql   					 = $obras->listaSql( $filtro );
$cabecalho				 = $obras->cabecalhoListaSql();
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
?>
	