<?php
$empid = $_SESSION['obras2']['empid'];
$obrid = $_REQUEST['obrid'];
$slcid = trim($_REQUEST['slcid']);

if($_REQUEST['carregarData']){
	header('content-type: text/html; charset=ISO-8859-1');	
	echo getDataFinalConvenio($_POST['dias'], $_POST['datainicio']);			
	exit;
}

function getDataFinalConvenio($dias, $data){
	$arData = explode('/', $data);

	$dia = $arData[0];
	$mes = $arData[1];
	$ano = $arData[2];
	$dataFinal = mktime(24*$dias, 0, 0, $mes, $dia, $ano);
	$dataFormatada = date('d/m/Y',$dataFinal);
	echo $dataFormatada;
}

if( $_REQUEST['requisicao'] == 'excluir' ){
	$sql = "delete from obras2.dados_solicitacao where dslid = {$_REQUEST['dslid']}";
	$db->executar($sql);
	$db->commit();
	
	$slcid = $_REQUEST['slcid'];
	$db->sucesso('principal/dadosSolicitacao', '&obrid='.$_REQUEST['obrid'].'&slcid='.$slcid);
	die;
}

if( $_POST['requisicao'] == 'salvar' ){
	
	$slcid = $_REQUEST['slcid'];
	$dslcomentario 		= $_POST['dslcomentario'] ? "'".$_POST['dslcomentario']."'" : 'null';
    $dsltipodesbloqueio = $_POST['dsltipodesbloqueio'];

	$sql = "INSERT INTO obras2.dados_solicitacao(slcid,  dslcomentario, usucpf, dsltipodesbloqueio)
			VALUES ($slcid, $dslcomentario, '{$_SESSION['usucpf']}', $dsltipodesbloqueio)";
	$db->executar($sql);
	$db->commit();
	$db->sucesso('principal/dadosSolicitacao', '&obrid='.$_REQUEST['obrid'].'&slcid='.$slcid);
	die;
}

$urlP = "/obras2/obras2.php?modulo=principal/dadosSolicitacao&acao=A&obrid=".$_REQUEST['obrid']."&slcid=".$_REQUEST['slcid'].'&tslid[]='.$_GET['tslid'][0];
print carregaAbaSolicitacao($urlP, $_REQUEST['obrid'], $_REQUEST['slcid']);

monta_titulo( "Resposta Solicitação", "" );

echo cabecalhoObra($obrid, 'simples');

$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);

$habilita = 'N';
if(possui_perfil( $pflcods )){
	$habilita = 'S';
}


?>
<html>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">
	$(document).ready(function(){
		$('#tr_obras1').css('display', 'none');
		
		if( $('#habilita').val() == 'N' ){ 
			$('input, select, textarea').attr('disabled', true);
			$('.img_add_foto').hide();
		}
		
	});
	
	function salvarDesbloqueio(){
		var erro = 1;

		if( $('[name="dslcomentario"]').val() == '' ){
			alert('O campo "Comentário" é obrigatório!');
			$('[name="dslcomentario"]').focus();
			return false;
		}

		$('#requisicao').val('salvar');
 		$('[name=formulario]').submit();
	}

	
	function excluirPedido( dslid ){
		window.location.href = window.location+'&requisicao=excluir&dslid='+dslid;
	}
</script>
<body>

<?

if( $habilita == 'S' ){
	$imagem = "'<img src=\"../imagens/excluir.gif\" onclick=\"excluirPedido('||d.dslid||')\" style=\"border:0; cursor:pointer;\" title=\"Excluir Desbloqueio\">'";
} else {
	$imagem = "'<img src=\"../imagens/excluir_01.gif\" style=\"border:0; cursor:pointer;\" title=\"Excluir Desbloqueio\">'";
}

$imagem .= " || CASE WHEN s.ckfid IS NOT NULL THEN ' <img title=\"Imprimir análise\" src=\"../imagens/print.png\" style=\"border:0; cursor:pointer;\" onclick=\"imprimirQuestionarioRespondido('|| qr.queid ||' , '|| qr.qrpid ||', '|| s.ckfid ||', '||s.obrid ||' );\" class=\"imprimir_questionario\">' ELSE '' END";

$sql = "SELECT 
		    $imagem as acoes,

		    CASE
		      WHEN dsltipodesbloqueio = 'D' THEN 'Deferido'
		      WHEN dsltipodesbloqueio = 'I' THEN 'Indeferido'
		      WHEN dsltipodesbloqueio = 'F' THEN 'Diligência'
		      ELSE '' END
		    ,
		  	 d.dslcomentario,
		  	d.dslpendencias,
		  	us.usunome,
		  	TO_CHAR(d.dsldata, 'DD/MM/YYYY')
		FROM 
			obras2.dados_solicitacao d
			inner join obras2.solicitacao s ON s.slcid = d.slcid
		  	inner join seguranca.usuario us on us.usucpf = d.usucpf
			LEFT JOIN obras2.checklistfnde ck ON ck.ckfid = s.ckfid
			LEFT JOIN questionario.questionarioresposta qr ON qr.qrpid = ck.qrpid
		WHERE
			d.slcid = {$_REQUEST['slcid']}
        ORDER BY d.dsldata DESC
        ";

$cabecalho = array('Ações', 'Análise da Solicitação', 'Comentário', 'Pendência', 'Usuário', 'Data');
$arrHeighTds = array('05%', '10%', '40%', '50%', '10%');
$db->monta_lista($sql, $cabecalho, 1000000, 5, 'N', 'center', '', '', $arrHeighTds);
?>
</body>
</html>
<script type="text/javascript">
    function imprimirQuestionarioRespondido(queid, qrpid, ckfid, obrid){
        return windowOpen('/obras2/obras2.php?modulo=principal/questionarioImpressaoChecklistValid&acao=A&queid=' + queid + '&qrpid='+qrpid+'&ckfid='+ckfid+'&obrid='+obrid ,'blank',
            'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
    }
</script>