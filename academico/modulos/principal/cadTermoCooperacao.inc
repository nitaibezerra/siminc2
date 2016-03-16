<?php

/**
 * Sistema Simec
 * Setor responsável: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Henrique
 * Programador: Gustavo Fernandes da Guarda
 */

require_once APPRAIZ . "academico/classes/TermoCooperacao.class.inc";
require_once APPRAIZ . 'academico/classes/Pagamento.class.inc';
require_once APPRAIZ . 'academico/classes/NaturezaDespesa.class.inc';
require_once APPRAIZ . "includes/classes/dateTime.inc";
include_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "academico/classes/Processo.class.inc";

//$_GET['tmcid'] = 1;
//unset($_SESSION['academico']['tmcid']);
$arrTipoMec  = unserialize( ARRAY_PERFIL_MEC );

if($_GET['tmcid'] && $_GET['tmcid'] != "") {
	$tmcid = $db->pegaUm("SELECT tmcid FROM academico.termocooperacao WHERE tmcid = {$_GET['tmcid']}");
	if(!$tmcid){
		echo "<script>
				alert('A Termo de Cooperação informado não existe!');
				history.back(-1);
			  </script>";
		die;
	}
	$_SESSION['academico']['tmcid'] = $_GET['tmcid'];
}

if($_SESSION['academico']['tmcid']){
	$tmcid = $_SESSION['academico']['tmcid'];
} else {
	$tmcid = $_GET['tmcid'];
}

$obTermoCooperacao = new TermoCooperacao($tmcid);

$docid = tcPegarDocid($tmcid);

$estadoAtual = pegarEstadoAtual( $docid );

if( $estadoAtual == WF_EM_PRESTACAO_CONTAS )
{
	$prcid = $db->pegaUm("SELECT prcid FROM academico.processo WHERE tmcid = ".$tmcid);

	if($prcid){
		echo "<script>
				window.location.href = 'academico.php?modulo=principal/cumprimentoObjeto&acao=A&prcid=$prcid';
			  </script>";
		die;
	}
}

$habilitado = 'N';
$habilitadoTermoOrc = 'S';

if($_POST['requisicao'] == 'salvarTermoCampus'){
	extract($_POST);
	
 	$tcavalor = str_replace(".", "", $tcavalor);
	$tcavalor = str_replace(",", ".", $tcavalor);
	
	if( !$tcaid ){
		$sql = "INSERT INTO academico.termocampus(tmcid, entid, tcaobjeto, ndpid, tcavalor) 
				VALUES ($tmcid, $entidcampus, '$tcaobjeto', $ndpcod, $tcavalor) RETURNING tcaid";	
		$tcaidTemp = $db->pegaUm( $sql );
		$tcaid = ($tcaid) ? $tcaid : $tcaidTemp;
	} else {
		$sql = "UPDATE academico.termocampus SET
				  entid = $entidcampus,
				  tcaobjeto = '$tcaobjeto',
				  ndpid = $ndpcod,
				  tcavalor = $tcavalor
				WHERE tcaid = $tcaid";		
		$db->executar( $sql );
	}
	
	if($db->commit()){
		$sql = "select ndpcod || ' - ' || ndpdsc from public.naturezadespesa where ndpid = {$ndpcod}";
		$ndpcod = $db->pegaUm($sql);
		$arTermoCampus = array();
		$arTermoCampus['msg'] 			= 'sucesso';
		$arTermoCampus['tcaid'] 		= $tcaid;
		$arTermoCampus['entidcampus'] 	= $entidcampus;
		$arTermoCampus['tcaobjeto'] 	= $tcaobjeto;
		$arTermoCampus['ndpcod'] 		= utf8_encode($ndpcod);
		$arTermoCampus['tcavalor'] 		= number_format($tcavalor,2,',','.');
		echo simec_json_encode($arTermoCampus);
	} else{
		$arTermoCampus['msg'] = 'erro';
		$db->rollback();
	}
	unset($_POST);
	unset($obTermoCampus);
	die;
}

if($_POST['requisicao'] == 'carregarTermoCampus'){
	extract($_POST);
	
	$sql = "SELECT tcaid, tmcid, entid, tcaobjeto, ndpid, tcavalor
			FROM academico.termocampus WHERE tmcid = $tmcid";
	
	$arrTermo = $db->pegaLinha( $sql );
	
	/*$obNaturezaDespesa = new NaturezaDespesa();
	if($arrTermo['ndpid']){
		$ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($arrTermo['ndpid']);		
	}*/
	
	$arTermoCampus = array();
	$arTermoCampus['tcaid']  		= $arrTermo['tcaid'];
	$arTermoCampus['entidcampus'] 	= $arrTermo['entid'];
	$arTermoCampus['tcaobjeto'] 	= $arrTermo['tcaobjeto'];
	$arTermoCampus['ndpcod'] 		= $arrTermo['ndpid'];
	$arTermoCampus['tcavalor'] 		= number_format($arrTermo['tcavalor'], 2, ',', '.');
	echo simec_json_encode($arTermoCampus);
	die;
}

if($_POST['requisicao'] == 'excluirTermoCampus'){
	extract($_POST);
	if($tcaid){
		$sql = "DELETE FROM academico.termocampus WHERE tcaid = $tcaid";
		$db->executar( $sql );
		if($db->commit()){
			echo 'sucesso';		
		} else {
			$db->rollback();
		}		
	}
	die;
}

if($_POST['outras_informacoes'])
{
	
	extract($_POST);
	$obrid = explode('-', $obrid);
	$obrid = trim($obrid[0]);
	
	/*$pgsid = explode('-', $pgsid);
	$pgsid = trim($pgsid[0]);*/
	
	//$tmcnumtermo =  $tmcnum.'/'.$tmcano;
	
	if( $tmcid ){
		if(isset($entunicod)) $filtrounicod .= " unicod = '$entunicod', ";
		if(isset($plicod)) $filtrounicod .= " plicod = '$plicod', ";
		if(isset($tmcdtnotacredito)){
			$tmcdtnotacredito = $tmcdtnotacredito ? "'".formata_data_sql( $tmcdtnotacredito )."'" : 'null';
			$filtrounicod .= " tmcdtnotacredito = $tmcdtnotacredito, ";
		}
		if(isset($prtid)){
			$prtid = $prtid ? $prtid : 'null';
			$filtrounicod .= " prtid = $prtid, ";
		}
		if(isset($tmcnc)){
			$tmcnc = $tmcnc ? "'".$tmcnc."'" : 'null';
			$filtrounicod .= " tmcnc = $tmcnc, ";
		}
		if(isset($dirid)){
			$dirid = $dirid ? $dirid : 'null';
			$filtrounicod .= " dirid = $dirid, ";
		}
		if(isset($cooid)){
			$cooid = $cooid ? $cooid : 'null';
			$filtrounicod .= " cooid = $cooid, ";
		}
		//tmcnumtermo = '$tmcnumtermo'
		$sql = "UPDATE academico.termocooperacao SET
				  obrid = $obrid,
				  tmcobs = '".simec_htmlspecialchars($tmcobs, ENT_QUOTES)."',
				  $filtrounicod
				  pgsid = '$pgsid' 
				WHERE 
				  tmcid = $tmcid";
		
		$db->executar( $sql );
	} else {	
		$obTermoCooperacao = new TermoCooperacao($tmcid);
		$obTermoCooperacao->obrid 				= $obrid;
		$obTermoCooperacao->plicod 				= $plicod;
		$obTermoCooperacao->tmcnc 				= $tmcnc ? $tmcnc : null;
		$obTermoCooperacao->tmcobs 				= simec_htmlspecialchars($tmcobs, ENT_QUOTES);
		$obTermoCooperacao->unicod 				= $entunicod;
		$obTermoCooperacao->tmcdtnotacredito 	= $tmcdtnotacredito ? formata_data_sql( $tmcdtnotacredito ) : null;
		$obTermoCooperacao->prtid 				= $prtid;
		$obTermoCooperacao->pgsid 				= $pgsid;
		$obTermoCooperacao->salvar();
	}
	
	$prcid = $db->pegaUm("SELECT prcid FROM academico.processo WHERE tmcid = ".$tmcid);
	if( $prtid  ){
		$obProcesso = new Processo($prcid);
		$obProcesso->tmcid = $tmcid;
		$obProcesso->prtid = $prtid;
		$obProcesso->salvar();
	}

	if( $tmcid ){
		$sql = "DELETE FROM academico.termometa WHERE tmcid = $tmcid";
		$db->executar($sql);
		foreach ($mtsid as $v) {
			$sql = "INSERT INTO academico.termometa(tmcid, mtsid) 
					VALUES ($tmcid, $v)";
			$db->executar($sql);
		}
	}

	$obTermoCooperacao->commit();
	
	echo "<script>
		alert('Dados gravados com sucesso!');
		window.location.href = 'academico.php?modulo=principal/cadTermoCooperacao&acao=A&tmcid=".$tmcid."';
		</script>";
	die;
}

if($_POST['pesquisa'] == 'proponente' ){
	extract($_POST);
	
	$entidconcedente = $db->pegaUm("select entid from entidade.funcaoentidade where funid = '".FUNCAO_ENTIDADE_CONCEDENTE."' and fuestatus = 'A'");
	$entidrepconc    = $db->pegaUm("select entid from entidade.funcaoentidade where funid = '".FUNCAO_REP_ENTIDADE_CONCEDENTE."' and fuestatus = 'A'");
	$entid		     = $db->pegaUm("select e.entid from entidade.entidade e inner join entidade.funcaoentidade ef ON ef.entid = e.entid
										WHERE e.entstatus = 'A' AND ef.funid in ('".ACA_ID_ESCOLAS_TECNICAS."') and entunicod = '".$entidproponente."' and entstatus = 'A'");

	$obTermoCooperacao = new TermoCooperacao($tmcid);
	if(!$obTermoCooperacao->entidconcedente && $entidconcedente){
		$obTermoCooperacao->entidconcedente = $entidconcedente;
	}
	if(!$obTermoCooperacao->entidrepconc && $entidrepconc){
		$obTermoCooperacao->entidrepconc = $entidrepconc;
	}
	if(!$tmcid){
		$obTermoCooperacao->tmcdtinclusao = date('Y-m-d H:m:s');
	}	
	$obTermoCooperacao->entidproponente = $entid;
	/*$obTermoCooperacao->obrid 	= $obrid;
	 $obTermoCooperacao->plicod 	= $plicod;
	 $obTermoCooperacao->tmcnc 	= $tmcnc;*/
	//$obTermoCooperacao->tmcobs 	= simec_htmlspecialchars($tmcobs, ENT_QUOTES);
	//if( $docid ) $obTermoCooperacao->docid 	= $docid;
	$tmcnumtermo =  $tmcnum.'/'.$tmcano;
	$obTermoCooperacao->unicod 		= $entunicodp;
	$obTermoCooperacao->tmcnumtermo	= $tmcnumtermo;
	$tmcidTemp = $obTermoCooperacao->salvar();
	$obTermoCooperacao->commit();
	if(!$tmcid){
		$tmcid = $tmcidTemp;
		$_SESSION['academico']['tmcid'] = $tmcid;
	}
	
	$docid = tcCriarDocumento( $tmcid );		
	$db->executar( "UPDATE academico.termocooperacao SET docid = '$docid' WHERE tmcid = $tmcid" );
	$db->commit();

	echo "<script>
		alert('Dados gravados com sucesso!');
		window.location.href = 'academico.php?modulo=principal/cadTermoCooperacao&acao=A&tmcid=".$tmcid."';
		</script>";
	die;
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

//$obTermoOrcamento = new TermoOrcamento($_GET['tmoid']);
$obNaturezaDespesa = new NaturezaDespesa();
if($obTermoOrcamento->ndpid){
	$ndpcod = $obNaturezaDespesa->pegaCodNaturezaPorId($obTermoOrcamento->ndpid);
}

$db->cria_aba( $abacod_tela, $url, array() );

if( empty($_SESSION['academico']['tmcid']) ){
	
	$sequencial = $db->pegaUm("SELECT COALESCE(max(gspseq), '0') as sequencial FROM public.geradorsequencialpi WHERE gspidentificador = 'SETEC'");
	$sequencial = $sequencial + 1;

	$sql = "INSERT INTO public.geradorsequencialpi(gspseq, gspidentificador) 
			VALUES ('$sequencial', 'SETEC')";
	
	$db->executar( $sql );
	$db->commit();
}

?>
<style>
.trTermoCampus {
	background: #fff none repeat scroll 0%;
}
</style>
<script
	type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
$(document).ready(function() {

	$('#entnumcpfcnpjp').val(mascaraglobal('##.###.###/####-##',$('#entnumcpfcnpjp').val()));
	$('#entnumcpfcnpjrp').val(mascaraglobal('###.###.###-##',$('#entnumcpfcnpjrp').val()));
	$('#entnumcpfcnpjc').val(mascaraglobal('##.###.###/####-##',$('#entnumcpfcnpjc').val()));
	$('#entnumcpfcnpjrc').val(mascaraglobal('###.###.###-##',$('#entnumcpfcnpjrc').val()));

	$('.trTermoCampus')
		.live('mouseover',function(){
			$(this).css('background-color','#FFFFCC');
		})
		.live('mouseout',function(){
			$(this).css('background-color','#FFFFFF');
		});

	$('.salvarTermoCampus').click(function(){

		var $tmcid = '<?php echo $tmcid; ?>';
		
		if(!$('#tmcid').val() && !$tmcid){
			alert('É necessário cadastrar Entidade Proponente ou Concedente')
			return false;
		}
		
		var msg = "";

		if($('#entidcampus').val() == ''){
			msg += "O campo Campus é obrigatório.\n";
		}
		if($('#tcaobjeto').val() == ''){
			msg += "O campo Objeto é obrigatório.\n";
		}
		if($('#ndpcod').val() == ''){
			msg += "O campo Nat. Despesa é obrigatório.\n";
		}
		if($('#tcavalor').val() == ''){
			msg += "O campo Valor é obrigatório.";
		}

		if(msg){
			alert(msg);
			return false;
		}

		var tcaid = $('#tcaid').val();
		
		$.ajax({
			   type		: "POST",
			   url		: "academico.php?modulo=principal/cadTermoCooperacao&acao=A",
			   data		: 'requisicao=salvarTermoCampus&tmcid='+$tmcid+'&'+$('#formTermoCampus').serialize(),
			   async    : false,
			   dataType : 'json',
			   success	: function(data){
			   				//$('#divDebug').html(data);
							if(data.msg == 'sucesso'){
			   					var rowIndexTemp = 0;
								if($('#tbodyTermoCampus').children().not('#totalTermoCampus').length > 0){
									var rowIndexTemp = $('#tbodyTermoCampus').not('#totalTermoCampus').children().length + 1;
								}
								
								var tr = $('#tbodyTermoCampus').children().not('#totalTermoCampus').last();
								
								var html = '<tr class="trTermoCampus">';
									html +='	<td>'+data.entidcampus+'</td>';
									html +='	<td>'+data.tcaobjeto+'</td>';
									html +='	<td>'+data.ndpcod+'</td>';
									html +='	<td style="text-align: right;">'+data.tcavalor+'</td>';
									html +='	<td style="text-align: center; width: 50px" ><img border="0" class="alterarTermoCampus" id="'+data.tcaid+'" style="cursor: pointer;" title="Alterar" src="/imagens/check_p.gif">&nbsp;<img border="0" id="'+data.tcaid+'" class="excluirTermoCampus" title="Excluir" style="cursor: pointer;" src="/imagens/exclui_p.gif"></td>';
									html +='</tr>';
								
								if(rowIndexTemp > 0){
									if(tcaid){
										tr.after(html).remove();
									} else {
										tr.after(html);					
									}
								} else {
									//$('#tbodyTermoCampus').append(html);
									$('#tbodyTermoCampus').before(html);
								}
							} else if(data.msg == 'erro') {
								alert('Ocorreu algum erro.');						
							}
						  }
			 });

		limparCampos();
		calcularTotal();
		return false;
	});

	$('.alterarTermoCampus').live('click',function(){
		var tcaid = $(this).attr('id');
		if(tcaid){
			$.ajax({
				   type		: "POST",
				   url		: "academico.php?modulo=principal/cadTermoCooperacao&acao=A",
				   data		: 'requisicao=carregarTermoCampus&tcaid='+tcaid,
				   async    : false,
				   dataType : 'json',
				   success	: function(data){
				   					//$('#divDebug').html(data); return false;
				   					$('#tcaid').val(tcaid);
									$('#entidcampus').val(data.entidcampus);
									$('#tcaobjeto').val(data.tcaobjeto);
									$('#ndpcod').val(data.ndpcod);
									$('#tcavalor').val(data.tcavalor);
							  }
				 });
		}
		return false;
	});

	$('.excluirTermoCampus').live('click',function(){

		if(!confirm('Deseja realmente excluir este registro.')){
			return false;
		}
		
		var tr = $(this).closest('tr');
		
		var param = new Array();
			param.push({name : 'requisicao', value : 'excluirTermoCampus'}, 
					   {name : 'tcaid', value : $(this).attr('id')}
			);
		$.ajax({
			type	 : "POST",
			url		 : "academico.php?modulo=principal/cadTermoCooperacao&acao=A",
			data	 : param,
			async    : false,
			//dataType : 'json',
			success	 : function(data){
							if(trim(data) == 'sucesso'){
								tr.remove();
								calcularTotal();
							}
					   }
			 });
	});
});

function carregarInformacoes(entid){
	$('#pesquisa').val( 'proponente' );
	$('#entidproponente').val( entid );
	$('#formProponente').submit();
}

function carregaAno(ano){
	$('#ano').val( 'tmcano' );
}

function abrirPopupEntidadeCNPJ(funid, tipoEntidade){
	var tmcid = '<?php echo $_SESSION['academico']['tmcid']; ?>';
	if(tipoEntidade == 'entidadeProponente'){
		var entid = $('#entidproponente').val();
	}
		
	window.open('academico.php?modulo=principal/popupEntidadeCNPJ&acao=A&funid='+funid+'&tipoEntidade='+tipoEntidade+'&entid='+entid+'&tmcid='+tmcid,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=900,height=800');
}

function abrirPopupEntidadeCPF(funid, tipoEntidade){
	var tmcid = '<?php echo $_SESSION['academico']['tmcid']; ?>';
	if(tipoEntidade == 'entidadeRepProponente'){
		var entid = $('#entidrepprop').val();
	}
	window.open('academico.php?modulo=principal/popupEntidadeCPF&acao=A&funid='+funid+'&tipoEntidade='+tipoEntidade+'&entid='+entid+'&tmcid='+tmcid,'','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=700,height=800');
}

function limparCampos()
{
	$('#tcaid').val('');
	$('#entidcampus').val('');
	$('#tcaobjeto').val('');
	$('#ndpcod').val('');
	$('#tcavalor').val('');
	return false;
}

function calcularTotal()
{
	var total = 0;
	
	$('#tbodyTermoCampus').find('tr').each(function(){
		var valor = $(this).find('td').next().next().next().text();
			if(valor){
				valor = parseFloat(replaceAll(replaceAll(valor,".",""),",","."));
				total = total + valor;
			}
	});
	
	$('#tdTotal').html("<strong>"+mascaraglobal('###.###.###.###,##',total.toFixed(2).replace(".",","))+"</strong>");
}

function carregaLink(){
	var obrid = document.getElementById('obrid').value;
	if( obrid != '' ){
		obrid = obrid.split("-");
		obrid = trim(obrid[0]);
		
		window.open('academico.php?modulo=principal/documentosObras&acao=A&obrid='+obrid,'documento','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=700');
	}
	
	//academico.php?modulo=principal/documentosObras&acao=A
}

function salvarOutrasInformacoes()
{
	var msg = "";

	if($('#obrid').val() == ''){
		msg += "O campo Obra é obrigatório.\n";
	}
	if($('#plicod').val() == ''){
		msg += "O campo PI é obrigatório.\n";
	}
	if($('#tmcnc').val() == ''){
		msg += "O campo Número da NC é obrigatório.\n";
	}
	if($('#prtid').val() == ''){
		msg += "O campo Portaria N° é obrigatório.\n";
	}
	if($('#pgsid').val() == ''){
		msg += "O campo Programa é obrigatório.\n";
	}
	
	selectAllOptions( document.getElementById( 'mtsid' ) );
	var mtsid	  = document.getElementById("mtsid");
	
	var j = mtsid.options.length;
	if( mtsid.options[0].value == '' ){
		msg += 'O campo Meta é obrigatório.\n';	
	}
	
	if($('#dirid').val() == ''){
		msg += "O campo Diretoria é obrigatório.\n";
	}
	if($('#cooid').val() == ''){
		msg += "O campo Coordenação é obrigatório.\n";
	}
	
	if(msg){
		alert(msg);
		return false;
	}
	
	$('#formulario').submit();
}

function enviarCoordenacao(docid, estadoAtual, tmcid){
	window.open('academico.php?modulo=principal/popupFluxoWorkflow&acao=A&docid='+docid+'&estado='+estadoAtual+'&tmcid='+tmcid,'fluxoworkflow','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=600,height=300');
}

function wf_exibirHistoricoTermo( docid ){
	var url = 'http://<?php echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php' +
		'?modulo=principal/tramitacao' +
		'&acao=C' +
		'&docid=' + docid;
	window.open(
		url,
		'alterarEstado',
		'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
	);
}
</script>
<?php
if($obTermoCooperacao->entidproponente){

	$sql = "select  e.entid,
					e.entnumcpfcnpj as entnumcpfcnpjp, 
					e.entnome as entnomep,
					e.entemail as entemailp, 
					e.entnumdddresidencial as entnumdddresidencialp,
					e.entnumresidencial as entnumresidencialp,
					e.entnumfax as entnumfaxp,
					en.endcep as endcepp,
					en.endlog as endlogp,
					en.endbai as endbaip,
					en.estuf as estufp,
					m.mundescricao as mundescricaoc,
					e.entunicod
				from  entidade.entidade e 
					left join entidade.endereco en on e.entid = en.entid
					left join territorios.municipio m on en.muncod = m.muncod
				where e.entid = '".$obTermoCooperacao->entidproponente."'";

	$arEntidadeProponente = $db->pegaLinha($sql);
}
$arEntidadeProponente = ($arEntidadeProponente) ? $arEntidadeProponente : array();
?>
<table align="center" border="0" class="tabela" cellpadding="3"
	cellspacing="1">
	<tr class="SubTituloEsquerda" style="background-color: #cccccc">
		<td colspan="3"><strong>DADOS DO ORGÃO OU ENTIDADE PROPONENTE</strong></td>
	</tr>
	<tr>
		<td
			style="padding: 15px; background-color: #e9e9e9; color: #404040; vertical-align: top;"
			colspan="2">
		<form method="post" name="formProponente" id="formProponente"><input
			type="hidden" name="entidproponente" id="entidproponente"
			value="<?php echo $obTermoCooperacao->entidproponente;?>" /> 
			<input type="hidden" name="pesquisa" id="pesquisa" value="1" />
		<table cellpadding="3" cellspacing="0" border="0">
			<tr>
				<td valign="bottom" colspan="4">1 Nº Termo <br />
				<?php
				$tmcnumtermo =  $sequencial ? $sequencial = str_pad( $sequencial, 6, 0, STR_PAD_LEFT ).'/'.date('Y') : $obTermoCooperacao->tmcnumtermo;
				if( empty($tmcid) ){
					$arNumTermo = explode('/', $tmcnumtermo);
					$tmcnum = $arNumTermo[0];
					$tmcano = $arNumTermo[1];
					$tmcnump = $tmcnum;
					
					echo campo_texto('tmcnump', 'N', 'N', '', 10, 10, '', '', '', '','', "id='tmcnump'");
					echo ' /&nbsp;';
					$sql = "SELECT distinct
								rofano as codigo,
								rofano as descricao
							FROM financeiro.execucao";
					$db->monta_combo("tmcano", $sql, "S", "", 'carregaAno();', '', '', '', 'N','tmcano');
				} else {
					echo campo_texto('tmcnumtermo', 'N', 'N', '', 20, 10, '', '', '', '','', "id='tmcnumtermo'");
				}
				
				?><input type="hidden" name="tmcnum" id="tmcnum" value="<?=$tmcnum; ?>" />
				</td>
				<td valign="bottom" colspan="3">2 Institutos Federais <br />
				<?php
				$arrEntid = array();
				if( !$db->testa_superuser() ){
					$arrEntid = $db->carregarColuna("SELECT DISTINCT entid FROM academico.usuarioresponsabilidade
																	WHERE usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A' and entid is not null");
					$filtro =  $arrEntid ? " and e.entid in (".implode(',', $arrEntid).")" : '';
				}

				$sql = "SELECT
											e.entunicod as codigo,
											e.entnome as descricao
										FROM
											entidade.entidade e
											inner join entidade.funcaoentidade ef ON ef.entid = e.entid
										WHERE
											e.entstatus = 'A' AND ef.funid in ('".ACA_ID_ESCOLAS_TECNICAS."') ".$filtro."
										ORDER BY
											e.entsig, e.entnome ";
				
				if( sizeof($arrEntid) == 1 ){
					$arrEntnome = $db->pegaLinha( $sql );
					$entunicodp = $arrEntnome['descricao'];
					
					if( empty($_SESSION['academico']['tmcid']) ){
						echo "<script>carregarInformacoes('".$arrEntnome['codigo']."');</script>";
					}
					echo campo_texto( 'entunicodp', 'N', 'N', '', 100, 200, '', '', '', '','', "id='entunicodp'" );
				} else {
					/*$entnomep = $arEntidadeProponente['entnomep'];*/
					$entunicodp = $obTermoCooperacao->unicod? $obTermoCooperacao->unicod : '';
					$db->monta_combo("entunicodp", $sql, "S", "Todas as Instituições", 'carregarInformacoes', '', '', '', 'N','entunicodp');
				}
				?></td>
			</tr>
			<tr>
				<td valign="bottom" colspan="4">3 CNPJ <br />
				<?php
				$entnumcpfcnpjp = $arEntidadeProponente['entnumcpfcnpjp'];
				echo campo_texto( 'entnumcpfcnpjp', 'N', $habilitado, '', 20, 200, '', '', '', '','', "id='entnumcpfcnpjp'" );
				?> <!--  <span style="cursor:pointer" onclick="abrirPopupEntidadeCNPJ(<?php echo FUNCAO_ENTIDADE_PROPONENTE; ?>,'entidadeProponente');"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /></span>-->
				</td>
				<td valign="bottom" colspan="3">4 Razão Social <br />
				<?php
				$entnomep = $arEntidadeProponente['entnomep'];
				echo campo_texto( 'entnomep', 'N', $habilitado, '', 100, 200, '', '', '', '','', "id='entnomep'" );
				?></td>
			</tr>
			<tr>
				<td valign="bottom" colspan="3">5 Endereço <br />
				<?php
				$endlogp = $arEntidadeProponente['endlogp'];
				echo campo_texto( 'endlogp', 'N', $habilitado, '', 20, 200, '', '', '', '','', "id='endlogp'" );
				?></td>
				<td valign="bottom" colspan="3">6 Bairro ou Distrito <br />
				<?php
				$endbaip = $arEntidadeProponente['endbaip'];
				echo campo_texto( 'endbaip', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='endbaip'" );
				?></td>
				<td valign="bottom">7 Município <br />
				<?php
				$mundescricaop = $arEntidadeProponente['mundescricaop'];
				echo campo_texto( 'mundescricaop', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='mundescricaop'" );
				?></td>
			</tr>
			<tr>
				<td valign="bottom">8 UF <br />
				<?php
				$estufp = $arEntidadeProponente['estufp'];
				echo campo_texto( 'estufp', 'N', $habilitado, '', 1, 200, '', '', '', '','', "id='estufp'" );
				?></td>
				<td valign="bottom">9 CEP <br />
				<?php
				$endcepp = $arEntidadeProponente['endcepp'];
				echo campo_texto( 'endcepp', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='endcepp'" );
				?></td>
				<td valign="bottom">10 DDD <br />
				<?php
				$entnumdddresidencialp = $arEntidadeProponente['entnumdddresidencialp'];
				echo campo_texto( 'entnumdddresidencialp', 'N', $habilitado, '', 1, 200, '', '', '', '','', "id='entnumdddresidencialp'" );
				?></td>
				<td valign="bottom">11 Telefone <br />
				<?php
				$entnumresidencialp = $arEntidadeProponente['entnumresidencialp'];
				echo campo_texto( 'entnumresidencialp', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='entnumresidencialp'" );
				?></td>
				<td valign="bottom">12 FAX <br />
				<?php
				$entnumfaxp = $arEntidadeProponente['entnumfaxp'];
				echo campo_texto( 'entnumfaxp', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='entnumfaxp'" );
				?></td>
				<td valign="bottom">13 E-Mail <br />
				<?php
				$entemailp = $arEntidadeProponente['entemailp'];
				echo campo_texto( 'entemailp', 'N', $habilitado, '', 20, 200, '', '', '', '','', "id='entemailp'" );
				?></td>
				<td valign="bottom">14 Código Unidade Gestora <br />
				<?php
				$municipio = $arEntidadeProponente['municipio'];
				echo campo_texto( 'municipio', 'N', $habilitado, '', 10, 200, '', '', '', '','', "id='municipio'" );
				?></td>
				<td valign="bottom">15 Código Da Gestão <br />
				<?php
				$municipio = $arEntidadeProponente['municipio'];
				echo campo_texto( 'municipio', 'N', $habilitado, '', 10, 200, '', '', '', '','', "id='municipio'" );
				?></td>
			</tr>
		</table>
		</form>
		</td>
		<td valign="top"
			style="background-color: #e9e9e9; color: #404040; text-align: right;">
			<?php
			/*if( $estadoAtual == WF_EM_ANALISE_DIRETORIA ){
				$modificacao = wf_pegarUltimaDataModificacao( $docid );
				$usuario = wf_pegarUltimoUsuarioModificacao( $docid );
				$comentario = trim( substr( wf_pegarComentarioEstadoAtual( $docid ), 0, 50 ) ) . "...";
				?>
				<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
                    	<td style="font-size: 7pt; text-align: center;">
                        	<span title="Cadastramento de parecer"> <strong>estado atual</strong> </span>
                        </td>
					</tr>
                    <tr style="text-align: center;">
                    	<td style="font-size: 7pt; text-align: center; cursor: pointer;"><a id="enviarcoordenacao" title="Enviar para Análise da Coordenação" 
                    			onclick="enviarCoordenacao(<?=$docid; ?>, <?=$estadoAtual; ?>, <?=$tmcid ?>);">Enviar para Análise da Coordenação</a></td>
					</tr>
					<tr style="background-color: #c9c9c9; text-align:center;">
						<td style="font-size:7pt; text-align:center;">
							<span title="estado atual">
								<b>histórico</b>
							</span>
						</td>
					</tr>
					<tr style="text-align:center;">
						<td style="font-size:7pt; border-top: 2px solid #d0d0d0;">
							<img
								style="cursor: pointer;"
								src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/fluxodoc.gif"
								title="<?php echo $usuario['usunome'] . " - " . $modificacao . " - " . simec_htmlentities( $comentario ); ?>"
								onclick="wf_exibirHistoricoTermo( '<?php echo $docid ?>' );"
							/>
						</td>
					</tr>
				</table>
                <br />
				<?
			} else {*/
				if($tmcid) {
					$docid = tcPegarDocid($tmcid);
					wf_desenhaBarraNavegacao( $docid , array( 'tmcid' => $tmcid ) );
				}
			//}
			?></td>
	</tr>
</table>
			<?php
			//ver($obTermoCooperacao,d);
			if($obTermoCooperacao->entidrepprop){
				$sql = "select
						e.entnumcpfcnpj as entnumcpfcnpjrp, 
						e.entnome as entnomerp,
						e.entemail as entemailrp, 
						e.entnumdddresidencial as entnumdddresidencialrp,
						e.entnumresidencial as entnumresidencialrp,
						e.entnumfax as entnumfaxrp,
						en.endcep as endceprp,
						en.endlog as endlogrp,
						en.endbai as endbairp,
						en.estuf as estufrp,
						m.mundescricao as mundescricaorp,
						e.entnumrg as entnumrgrp,
						e.entorgaoexpedidor as entorgaoexpedidorrc
						--u.usufuncao
					from  entidade.entidade e 
						left join entidade.endereco en on e.entid = en.entid
						left join territorios.municipio m on en.muncod = m.muncod
					where e.entid = '{$obTermoCooperacao->entidrepprop}'";
				$arRepEntidadeProponente = $db->pegaLinha($sql);
			}
			$arRepEntidadeProponente = ($arRepEntidadeProponente) ? $arRepEntidadeProponente : array();
			?>
<table align="center" border="0" class="tabela" cellpadding="3"
	cellspacing="1">
	<tr class="SubTituloEsquerda" style="background-color: #cccccc">
		<td><strong>REPRESENTANTE LEGAL DO ORGÃO OU ENTIDADE PROPONENTE</strong></td>
	</tr>
	<tr>
		<td
			style="padding: 15px; background-color: #e9e9e9; color: #404040; vertical-align: top;"
			colspan="4">
		<form method="post" name="formRepProponente" id="formRepProponente"><input
			type="hidden" name="entidrepprop" id="entidrepprop"
			value="<?php echo $obTermoCooperacao->entidrepprop; ?>" /> <input
			type="hidden" name="pesquisa" value="1" />
		<table cellpadding="3" cellspacing="0" border="0">
			<tr>
				<td valign="bottom" colspan="3">16 CPF <br />
				<?php
				$entnumcpfcnpjrp = $arRepEntidadeProponente['entnumcpfcnpjrp'];
				//$entnumcpfcnpjrp = '991.393.081-20';
				echo campo_texto( 'entnumcpfcnpjrp', 'N', $habilitado, '', 20, 14, '###.###.###-##', '', '', '','', "id='entnumcpfcnpjrp'" );
				?> <span style="cursor: pointer"
					onclick="abrirPopupEntidadeCPF(<?php echo FUNCAO_REP_ENTIDADE_PROPONENTE; ?>,'entidadeRepProponente');"><img
					src="/imagens/gif_inclui.gif" align="absmiddle"
					style="border: none" /></span></td>
				<td valign="bottom" colspan="3">17 Nome do representante legal <br />
				<?php
				$entnomerp = $arRepEntidadeProponente['entnomerp'];
				echo campo_texto( 'entnomerp', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='entnomerp'" );
				?></td>
			</tr>
			<tr>
				<td valign="bottom" colspan="3">18 Endereço <br />
				<?php
				$endlogrp = $arRepEntidadeProponente['endlogrp'];
				echo campo_texto( 'endlogrp', 'N', $habilitado, '', 20, 200, '', '', '', '','', "id='endlogrp'" );
				?></td>
				<td valign="bottom" colspan="3">19 Bairro ou Distrito <br />
				<?php
				$endbairp = $arRepEntidadeProponente['endbairp'];
				echo campo_texto( 'endbairp', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='endbairp'" );
				?></td>
				<td valign="bottom">20 Município <br />
				<?php
				$mundescricaorp = $arRepEntidadeProponente['mundescricaorp'];
				echo campo_texto( 'mundescricaorp', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='mundescricaorp'" );
				?></td>
			</tr>
			<tr>
				<td valign="bottom">21 UF <br />
				<?php
				$estufrp = $arRepEntidadeProponente['estufrp'];
				echo campo_texto( 'estufrp', 'N', $habilitado, '', 1, 200, '', '', '', '','', "id='estufrp'" );
				?></td>
				<td valign="bottom">22 CEP <br />
				<?php
				$endceprp = $arRepEntidadeProponente['endceprp'];
				echo campo_texto( 'endceprp', 'N', $habilitado, '', 10, 200, '', '', '', '','', "id='endceprp'" );
				?></td>
				<td valign="bottom">23 DDD <br />
				<?php
				$entnumdddresidencialrp = $arRepEntidadeProponente['entnumdddresidencialrp'];
				echo campo_texto( 'entnumdddresidencialrp', 'N', $habilitado, '', 1, 200, '', '', '', '','', "id='entnumdddresidencialrp'" );
				?></td>
				<td valign="bottom">24 Telefone <br />
				<?php
				$entnumresidencialrp = $arRepEntidadeProponente['entnumresidencialrp'];
				echo campo_texto( 'entnumresidencialrp', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='entnumresidencialrp'" );
				?></td>
				<td valign="bottom">25 FAX <br />
				<?php
				$entnumfaxrp = $arRepEntidadeProponente['entnumfaxrp'];
				echo campo_texto( 'entnumfaxrp', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='entnumfaxrp'" );
				?></td>
				<td valign="bottom">26 E-Mail <br />
				<?php
				$entemailrp = $arRepEntidadeProponente['entemailrp'];
				echo campo_texto( 'entemailrp', 'N', $habilitado, '', 20, 200, '', '', '', '','', "id='entemailrp'" );
				?></td>
				<td valign="bottom">27 Número da cédula de identidade <br />
				<?php
				$entnumrgrp = $arRepEntidadeProponente['entnumrgrp'];
				echo campo_texto( 'entnumrgrp', 'N', $habilitado, '', 50, 200, '', '', '', '','', "id='entnumrgrp'" );
				?></td>
			</tr>
			<tr>
				<td valign="bottom">28 Data de emissão <br />
				<?php
				$municipio = $arRepEntidadeProponente['municipio'];
				echo campo_texto( 'municipio', 'N', $habilitado, '', 10, 200, '', '', '', '','', "id='municipio'" );
				?></td>
				<td valign="bottom">29 Órgão Expedidor <br />
				<?php
				$entorgaoexpedidorrp = $arRepEntidadeProponente['entorgaoexpedidorrp'];
				echo campo_texto( 'entorgaoexpedidorrp', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='entorgaoexpedidorrp'" );
				?></td>
				<td valign="bottom">30 Matrícula <br />
				<?php
				$municipio = $arRepEntidadeProponente['municipio'];
				echo campo_texto( 'municipio', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='municipio'" );
				?></td>
				<td valign="bottom">31 Cargo <br />
				<?php
				$usufuncao = $arRepEntidadeProponente['usufuncao'];
				echo campo_texto( 'usufuncao', 'N', $habilitado, '', 15, 200, '', '', '', '','', "id='usufuncao'" );
				?></td>
			</tr>
		</table>
		</form>
		</td>
	</tr>
</table>
				<?php
				$entidconcedente = ($obTermoCooperacao->entidconcedente) ? $obTermoCooperacao->entidconcedente : '606963';
				$sql = "select  e.entid,
					e.entnumcpfcnpj as entnumcpfcnpjc, 
					e.entnome as entnomec,
					e.entemail as entemailc, 
					e.entnumdddresidencial as entnumdddresidencialc,
					e.entnumresidencial as entnumresidencialc,
					e.entnumfax as entnumfaxc,
					en.endcep as endcepc,
					en.endlog as endlogc,
					en.endbai as endbaic,
					en.estuf as estufc,
					m.mundescricao as mundescricaoc
				from  entidade.entidade e 
					left join entidade.endereco en on e.entid = en.entid
					left join territorios.municipio m on en.muncod = m.muncod
				where e.entid = '{$entidconcedente}'";
				$arEntidadeConcedente = $db->pegaLinha($sql);
				$arEntidadeConcedente = ($arEntidadeConcedente) ? $arEntidadeConcedente : array();
				?>
<table align="center" border="0" class="tabela" cellpadding="3"
	cellspacing="1">
	<tr class="SubTituloEsquerda" style="background-color: #cccccc">
		<td><strong>DADOS DO ÓRGÃO OU ENTIDADE CONCEDENTE</strong></td>
	</tr>
	<tr>
		<td
			style="padding: 15px; background-color: #e9e9e9; color: #404040; vertical-align: top;"
			colspan="4">
		<form method="post" name="formConcedente" id="formConcedente"><input
			type="hidden" name="entidconcedente" id="entidconcedente"
			value="<?php echo $obTermoCooperacao->entidconcedente ?>" /> <input
			type="hidden" name="pesquisa" value="1" />
		<table cellpadding="3" cellspacing="0" border="0">
			<tr>
				<td valign="bottom" colspan="4">1 CNPJ <br />
				<?php
				$entnumcpfcnpjc = $arEntidadeConcedente['entnumcpfcnpjc'];
				echo campo_texto( 'entnumcpfcnpjc', 'N', $habilitado, '', 20, 200, '', '', '', '','', "id='entnumcpfcnpjc'" );
				?></td>
				<td valign="bottom" colspan="3">2 Razão Social <br />
				<?php
				$entnomec = $arEntidadeConcedente['entnomec'];
				echo campo_texto( 'entnomec', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='entnomec'" );
				?></td>
			</tr>
			<tr>
				<td valign="bottom" colspan="3">3 Endereço <br />
				<?php
				$endlogp = $arEntidadeConcedente['endlogc'];
				echo campo_texto( 'endlogc', 'N', $habilitado, '', 20, 200, '', '', '', '','', "id='endlogc'" );
				?></td>
				<td valign="bottom" colspan="3">4 Bairro ou Distrito <br />
				<?php
				$endbaic = $arEntidadeConcedente['endbaic'];
				echo campo_texto( 'endbaic', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='endbaic'" );
				?></td>
				<td valign="bottom">5 Município <br />
				<?php
				$mundescricaoc = $arEntidadeConcedente['mundescricaoc'];
				echo campo_texto( 'mundescricaoc', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='mundescricaoc'" );
				?></td>
			</tr>
			<tr>
				<td valign="bottom">6 UF <br />
				<?php
				$estufc = $arEntidadeConcedente['estufc'];
				echo campo_texto( 'estufc', 'N', $habilitado, '', 1, 200, '', '', '', '','', "id='estufc'" );
				?></td>
				<td valign="bottom">7 CEP <br />
				<?php
				$endcepc = $arEntidadeConcedente['endcepc'];
				echo campo_texto( 'endcepc', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='endcepc'" );
				?></td>
				<td valign="bottom">8 DDD <br />
				<?php
				$entnumdddresidencialc = $arEntidadeConcedente['entnumdddresidencialc'];
				echo campo_texto( 'entnumdddresidencialc', 'N', $habilitado, '', 1, 200, '', '', '', '','', "id='entnumdddresidencialc'" );
				?></td>
				<td valign="bottom">9 Telefone <br />
				<?php
				$entnumresidencialp = $arEntidadeConcedente['entnumresidencialp'];
				echo campo_texto( 'entnumresidencialp', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='entnumresidencialp'" );
				?></td>
				<td valign="bottom">10 FAX <br />
				<?php
				$entnumfaxc = $arEntidadeConcedente['entnumfaxc'];
				echo campo_texto( 'entnumfaxc', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='entnumfaxc'" );
				?></td>
				<td valign="bottom">11 E-Mail <br />
				<?php
				$entemailc = $arEntidadeConcedente['entemailc'];
				echo campo_texto( 'entemailc', 'N', $habilitado, '', 20, 200, '', '', '', '','', "id='entemailc'" );
				?></td>
				<td valign="bottom">12 Código Unidade Gestora <br />
				<?php
				$municipio = $arEntidadeConcedente['municipio'];
				echo campo_texto( 'municipio', 'N', $habilitado, '', 10, 200, '', '', '', '','', "id='municipio'" );
				?></td>
				<td valign="bottom">13 Código Da Gestão <br />
				<?php
				$municipio = $arEntidadeConcedente['municipio'];
				echo campo_texto( 'municipio', 'N', $habilitado, '', 10, 200, '', '', '', '','', "id='municipio'" );
				?></td>
			</tr>
		</table>
		</form>
		</td>
	</tr>
</table>
				<?php
				$entidrepconc = ($obTermoCooperacao->entidrepconc) ? $obTermoCooperacao->entidrepconc : '607205';
				$sql = "select
					e.entnumcpfcnpj as entnumcpfcnpjrc, 
					e.entnome as entnomerc,
					e.entemail as entemailrc, 
					e.entnumdddresidencial as entnumdddresidencialrc,
					e.entnumresidencial as entnumresidencialrc,
					e.entnumfax as entnumfaxrc,
					en.endcep as endceprc,
					en.endlog as endlogrc,
					en.endbai as endbairc,
					en.estuf as estufrc,
					m.mundescricao as mundescricaorc,
					e.entnumrg as entnumrgrc,
					e.entorgaoexpedidor as entorgaoexpedidorrc
					--u.usufuncao
				from  entidade.entidade e 
					left join entidade.endereco en on e.entid = en.entid
					left join territorios.municipio m on en.muncod = m.muncod
				where e.entid = '$entidrepconc'";
				$arRepEntidadeConcedente = $db->pegaLinha($sql);
				$arRepEntidadeConcedente = ($arRepEntidadeConcedente) ? $arRepEntidadeConcedente : array();
				?>
<table align="center" border="0" class="tabela" cellpadding="3"
	cellspacing="1">
	<tr class="SubTituloEsquerda" style="background-color: #cccccc">
		<td><strong>REPRESENTANTE LEGAL DO ORGÃO OU ENTIDADE CONCEDENTE</strong></td>
	</tr>
	<tr>
		<td
			style="padding: 15px; background-color: #e9e9e9; color: #404040; vertical-align: top;"
			colspan="4">
		<form method="post" name="formRepConcedente" id="formRepConcedente"><input
			type="hidden" name="entidrepconc" id="entidrepconc"
			value="<?php echo $obTermoCooperacao->entidrepconc; ?>" /> <input
			type="hidden" name="pesquisa" value="1" />
		<table cellpadding="3" cellspacing="0" border="0">
			<tr>
				<td valign="bottom" colspan="3">14 CPF <br />
				<?php
				$entnumcpfcnpjrc = $arRepEntidadeConcedente['entnumcpfcnpjrc'];
				echo campo_texto( 'entnumcpfcnpjrc', 'N', $habilitado, '', 20, 14, '###.###.###-##', '', '', '','', "id='entnumcpfcnpjrc'" );
				?></td>
				<td valign="bottom" colspan="3">15 Nome do representante legal <br />
				<?php
				$entnomerc = $arRepEntidadeConcedente['entnomerc'];
				echo campo_texto( 'entnomerc', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='entnomerc'" );
				?></td>
			</tr>
			<tr>
				<td valign="bottom" colspan="3">16 Endereço <br />
				<?php
				$endlogrc = $arRepEntidadeConcedente['endlogrc'];
				echo campo_texto( 'endlogrc', 'N', $habilitado, '', 20, 200, '', '', '', '','', "id='endlogrc'" );
				?></td>
				<td valign="bottom" colspan="3">17 Bairro ou Distrito <br />
				<?php
				$endbairc = $arRepEntidadeConcedente['endbairc'];
				echo campo_texto( 'endbairc', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='endbairc'" );
				?></td>
				<td valign="bottom">18 Município <br />
				<?php
				$mundescricaorc = $arRepEntidadeConcedente['mundescricaorc'];
				echo campo_texto( 'mundescricaorc', 'N', $habilitado, '', 30, 200, '', '', '', '','', "id='mundescricaorc'" );
				?></td>
			</tr>
			<tr>
				<td valign="bottom">19 UF <br />
				<?php
				$estufrc = $arRepEntidadeConcedente['estufrc'];
				echo campo_texto( 'estufrc', 'N', $habilitado, '', 1, 200, '', '', '', '','', "id='estufrc'" );
				?></td>
				<td valign="bottom">20 CEP <br />
				<?php
				$endceprc = $arRepEntidadeConcedente['endceprc'];
				echo campo_texto( 'endceprc', 'N', $habilitado, '', 10, 200, '', '', '', '','', "id='endceprc'" );
				?></td>
				<td valign="bottom">21 DDD <br />
				<?php
				$entnumdddresidencialrc = $arRepEntidadeConcedente['entnumdddresidencialrc'];
				echo campo_texto( 'entnumdddresidencialrc', 'N', $habilitado, '', 1, 200, '', '', '', '','', "id='entnumdddresidencialrc'" );
				?></td>
				<td valign="bottom">22 Telefone <br />
				<?php
				$entnumresidencialrc = $arRepEntidadeConcedente['entnumresidencialrc'];
				echo campo_texto( 'entnumresidencialrc', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='entnumresidencialrc'" );
				?></td>
				<td valign="bottom">23 FAX <br />
				<?php
				$entnumfaxrc = $arRepEntidadeConcedente['entnumfaxrc'];
				echo campo_texto( 'entnumfaxrc', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='entnumfaxrc'" );
				?></td>
				<td valign="bottom">24 E-Mail <br />
				<?php
				$entemailrc = $arRepEntidadeConcedente['entemailrc'];
				echo campo_texto( 'entemailrc', 'N', $habilitado, '', 20, 200, '', '', '', '','', "id='entemailrc'" );
				?></td>
				<td valign="bottom">25 Número da cédula de identidade <br />
				<?php
				$entnumrgrc = $arRepEntidadeConcedente['entnumrgrc'];
				echo campo_texto( 'entnumrgrc', 'N', $habilitado, '', 50, 200, '', '', '', '','', "id='entnumrgrc'" );
				?></td>
			</tr>
			<tr>
				<td valign="bottom">26 Data de emissão <br />
				<?php
				$municipio = $arRepEntidadeConcedente['municipio'];
				echo campo_texto( 'municipio', 'N', $habilitado, '', 10, 200, '', '', '', '','', "id='municipio'" );
				?></td>
				<td valign="bottom">27 Órgão Expedidor <br />
				<?php
				$entorgaoexpedidorrc = $arRepEntidadeConcedente['entorgaoexpedidorrc'];
				echo campo_texto( 'entorgaoexpedidorrc', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='entorgaoexpedidorrc'" );
				?></td>
				<td valign="bottom">28 Matrícula <br />
				<?php
				$municipio = $arRepEntidadeConcedente['municipio'];
				echo campo_texto( 'municipio', 'N', $habilitado, '', 7, 200, '', '', '', '','', "id='municipio'" );
				?></td>
				<td valign="bottom">29 Cargo <br />
				<?php
				$usufuncao = $arRepEntidadeConcedente['usufuncao'];
				echo campo_texto( 'usufuncao', 'N', $habilitado, '', 15, 200, '', '', '', '','', "id='usufuncao'" );
				?></td>
			</tr>
		</table>
		</form>
		</td>
	</tr>
</table>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
	<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<table align="center" border="0" class="tabela" cellpadding="3"
	cellspacing="1">
	<tr class="SubTituloEsquerda" style="background-color: #cccccc">
		<td><strong>OUTRAS INFORMAÇÕES</strong></td>
	</tr>
	<tr>
		<td
			style="padding: 15px; background-color: #e9e9e9; color: #404040; vertical-align: top;"
			colspan="4">
		<form method="post" name="formulario" id="formulario">
			<input type="hidden" name="outras_informacoes" id="outras_informacoes" value="1" />
			<input type="hidden" name="tmcano" id="tmcano" value="<?=$tmcano; ?>" />
			<input type="hidden" name="tmcnum" id="tmcnum" value="<?=$tmcnum; ?>" />
		<table cellpadding="3" cellspacing="0" border="0">
			<tr>
				<td valign="bottom" colspan="2">Obra: <br />
				<?php
				$obTermoCooperacao = new TermoCooperacao($tmcid);
				$obrid = $obTermoCooperacao->obrid;
				if( $obrid ) $obrid = $db->pegaUm("SELECT obrid||' - '||obrdesc as codigo FROM obras.obrainfraestrutura WHERE obrid = $obrid");
				$filtroObras = " and entidunidade = ".$arEntidadeProponente['entid'];
				//$filtroObras = $_REQUEST['entunicod'] ? " and entidunidade = ".$arEntidadeProponente['entid'] : '';

				$sql = "select
							obrid||' - '||obrdesc as codigo 
					   	from 
					   		obras.obrainfraestrutura 
					   	where 
					   		obsstatus = 'A'
					   		AND orgid = 2 $filtroObras
					   	ORDER BY
					   		obrdesc ASC";
				texto_popup('obrid', $sql, 'Obra', 8, 100 );?> <a style="cursor: pointer;" onclick="carregaLink();">Checklist Obras</a>
					<input type="hidden" id="imitexto" value="<?php echo (($obTermoCooperacao->tmcobs) ? $obTermoCooperacao->tmcobs: '')?>" />
				</td>
			</tr>
			<?if( $estadoAtual == WF_EM_ANALISE_ORCAMENTARIA ){
				if( academico_possui_perfil( $arrTipoMec ) ){ ?>
				<tr>
					<td valign="bottom" colspan="2">PI: <br />
					<?php
					
					if( $_REQUEST['entunicod'] ){
					 	$filtrosPI = " and unicod = '".$_REQUEST['entunicod']."'";
					 } elseif( $arEntidadeProponente['entunicod'] ){
					 	$filtrosPI = " and unicod = '".$arEntidadeProponente['entunicod']."'";
					 }
					$sql_pi = "SELECT
									pliid as codigo, 
									plicod||' - '||plititulo as descricao
								FROM
									monitora.pi_planointerno
								WHERE
									plistatus = 'A'
									and plisituacao in ('A', 'C')
                                    --and pliano = '".date('Y')."'
                                    $filtrosPI";
	
	                $plicod = $obTermoCooperacao->plicod;
	                $db->monta_combo("plicod", $sql_pi, "S", "Selecione", '', '', '', '530', 'N','plicod'); ?></td>
				</tr>
			<?} ?>
			<tr>
				<td valign="bottom" width="20%">Número da NC: <br />
				<?php
				$tmcnc = $obTermoCooperacao->tmcnc;
				echo campo_texto( 'tmcnc', 'N', 'S', '', 20, 12, '', '', '', '','', "id='nc'" );
				?></td>
				<td valign="bottom">Data da Nota de Crédito: <br />
				<?php
				$tmcdtnotacredito = $obTermoCooperacao->tmcdtnotacredito ? formata_data( $obTermoCooperacao->tmcdtnotacredito ) : '';
				echo campo_data2('tmcdtnotacredito', 'N', 'S','Data da Nota de Crédito', '', '', '', $tmcdtnotacredito, '');
				?></td>
			</tr>
			<tr>
				<td valign="bottom" colspan="2">Portaria N°:<br/>
					<?php 
						$prtid = $obTermoCooperacao->prtid;
						$sql = "select prtid as codigo, prtdsc as descricao from academico.portaria where prtstatus = 'A'";
						$db->monta_combo("prtid", $sql, 'S', 'Selecione', '', '' );
					?>
				</td>
			</tr>
			<?} #fim do if que verifica o estado atual se e <> de elaboração  ?>
			<tr>
				<td valign="bottom" colspan="2">Programa:<br/>
					<?php 
						$pgsid = $obTermoCooperacao->pgsid;						
						$sql = "select pgsid as codigo, pgscod||' - '||pgsdsc as descricao from academico.programasetec where pgsstatus = 'A'";
						$db->monta_combo("pgsid", $sql, 'S', 'Selecione', '', '', '', '', '', 'pgsid' );
					?>
				</td>
			</tr>
			<tr>
				<td valign="bottom" colspan="2">Metas:<br/>
					<?php 
						$mtsid = $db->carregar("SELECT
													m.mtsid as codigo,
												    m.mtstitulo as descricao
												FROM
													academico.metasetec m
												    inner join academico.termometa t on t.mtsid = m.mtsid
												WHERE
													t.tmcid = $tmcid"); 
						$sql = "SELECT 
									mtsid as codigo,
								  	mtstitulo as descricao
								FROM academico.metasetec WHERE mtsstatus = 'A'";
		
						combo_popup( 'mtsid', $sql, '', '400x400', 0, array(), '', 'S', false, false, 10, 400 );
					?>
				</td>
			</tr>
			<?if( $estadoAtual == WF_EM_AVALIACAO ){ ?>
			<tr>
				<td valign="bottom" colspan="2">Diretoria:<br/>
					<?php 
						$dirid = $obTermoCooperacao->dirid;
						
						if(  academico_possui_perfil( PERFIL_DIRETORIA_SETEC ) ){
							$sql = "SELECT 
									  c.dirid as codigo,
									  c.dirdsc as descricao
									FROM 
									  academico.diretoria c
						              inner join academico.usuarioresponsabilidade ur on ur.dirid = c.dirid
									WHERE 
										c.dirstatus = 'A'
										and ur.rpustatus='A'
						                and ur.usucpf = '{$_SESSION['usucpf']}' AND 
									 	    ur.pflcod = ".PERFIL_DIRETORIA_SETEC;
						} else {							
							$sql = "SELECT dirid as codigo, dirdsc as descricao FROM academico.diretoria WHERE dirstatus = 'A'";
						}
						$db->monta_combo("dirid", $sql, 'S', 'Selecione', '', '', '', '550', '', 'dirid' );
					?>
				</td>
			</tr>
			<?} ?>
			<?if( $estadoAtual == WF_EM_ANALISE_DIRETORIA ){ ?>
			<tr>
				<td valign="bottom" colspan="2">Coordenação:<br/>
					<?php 
						$cooid = $obTermoCooperacao->cooid;
						
						if(  academico_possui_perfil( PERFIL_COORDENADOR_GERAL ) ){
							$sql = "SELECT 
									  c.cooid as codigo,
									  c.coodsc as descricao
									FROM 
									  academico.coordenacao c
						              inner join academico.usuarioresponsabilidade ur on ur.cooid = c.cooid
									WHERE 
										c.coostatus = 'A'
										and ur.rpustatus='A'
						                and ur.usucpf = '{$_SESSION['usucpf']}' AND 
									 	    ur.pflcod = ".PERFIL_COORDENADOR_GERAL;
						} else {							
							$sql = "SELECT cooid as codigo, coodsc as descricao FROM academico.coordenacao WHERE coostatus = 'A'";
						}						
						$db->monta_combo("cooid", $sql, 'S', 'Selecione', '', '', '', '550', '', 'cooid' );
					?>
				</td>
			</tr>
			<?} ?>
			<tr>
				<td colspan="2"><input type="button" value="Salvar" onclick="salvarOutrasInformacoes();" /></td>
			</tr>
			</tr>
		</table>
		</form>
		</td>
	</tr>
</table>
<?if($tmcid){ ?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr	class="SubTituloEsquerda" style="background-color: #cccccc">
		<td><strong>&nbsp;</strong></td>
	</tr>
	<tr>
		<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
			<form method="post" name="formTermoCampus" id="formTermoCampus">
				<input type="hidden" name="tmcid" id="tmcid" value="<?php echo $tmcid; ?>" />
				<input type="hidden" name="tcaid" id="tcaid" value="<?php echo $tcaid; ?>" />
				<table class="tabela" id="tabTermoCampus" align="center" bgcolor="#000" background="#dcdcdc" border="0" cellpadding="3" cellspacing="1" width="100%">
					<tr>
						<td valign="top" class="SubTituloCentro" style="background-color: #dcdcdc"><strong>Campus</strong></td>
						<td valign="top" class="SubTituloCentro" style="background-color: #dcdcdc"><strong>Objeto</strong></td>
						<td valign="top" class="SubTituloCentro" style="background-color: #dcdcdc"><strong>Nat. da despesa</strong></td>
						<td valign="top" class="SubTituloCentro" style="background-color: #dcdcdc"><strong>Valor (em R$ 1,00)</strong></td>
						<td valign="top" class="SubTituloCentro" style="background-color: #dcdcdc"><strong>Ações</strong></td>
					</tr>
					<tr bgcolor="#FFFFFF">
						<td valign="middle" align="center">
						<?php
							
							$sql_campus = "SELECT
											e.entid as codigo,
											upper(e.entnome) as descricao
										FROM
											entidade.entidade e2
										INNER JOIN
											entidade.entidade e ON e2.entid = e.entid
										INNER JOIN
											entidade.funcaoentidade ef ON ef.entid = e.entid
										INNER JOIN
											entidade.funentassoc ea ON ea.fueid = ef.fueid 
										LEFT JOIN
											entidade.endereco ed ON ed.entid = e.entid
										LEFT JOIN
											territorios.municipio mun ON mun.muncod = ed.muncod
										WHERE
											ea.entid = ".$arEntidadeProponente['entid']." AND
											e.entstatus = 'A' AND ef.funid = ".ACA_ID_UNED."
										ORDER BY
											e.entnome";
							
							texto_popup( 'entidcampus', $sql_campus, 'Campus', 8, 15, '########', '', $complemento );
						?> 
						</td>
						<td valign="middle" align="center">
							<textarea id="tcaobjeto" name="tcaobjeto" rows="3" cols="50"></textarea>
						<?php
							/*$acacod = $obTermoOrcamento->acacod;
							$sql = "SELECT ac.acacod as codigo, ac.acadsc as descricao
						 			FROM monitora.acao ac 
						 			WHERE ac.prgano = '".$_SESSION['exercicio']."' AND ac.acastatus = 'A' AND ac.acasnrap = false  
						 			GROUP BY ac.acacod, ac.acadsc 
						 			ORDER BY ac.acacod";
							texto_popup( 'acacod', $sql, 'Ação', 8, 15, '########', '', $complemento );*/
						?> 
						</td>
						<td valign="middle" align="center">
						<?php
							$sql = "select ndpid as codigo, 
										   ndpdsc as descricao from public.naturezadespesa where ndpstatus = 'A'";
							texto_popup( 'ndpcod', $sql, 'Natureza', 8, 15, '########', '', $complemento );
						?> 
						
						</td>
						<td valign="middle" align="center">
							<?php 
								$tcavalor = ($tcavalor) ? number_format($tcavalor,2,",",".") : "" ;
								echo campo_texto( 'tcavalor', 'N', $habilitadoTermoOrc, '', 12, 500, '###.###.###.###,##', '', '', '','', "id='tcavalor'" );
							?>
						</td>
						<td valign="middle" align="center"><img src="/imagens/gif_inclui.gif" class="salvarTermoCampus" style="cursor: pointer;" border=0 title="Salvar" /> &nbsp; <a href="javascript: void(0);" onclick="limparCampos();"><img src="/imagens/borracha.gif" id="limpar" style="cursor: pointer;" border=0 title="Limpar" /></a> </td>
					</tr>
					<?php 
					if($tmcid){
						$sql = "select tmc.tcaid, 
								       nd.ndpcod||' - '||nd.ndpdsc as descnatureza,
								       tmc.tmcid, 
								       tmc.entid ||' - '|| e.entnome as desccampus,
								       tmc.tcaobjeto,
								       tmc.tcavalor							   
								from academico.termocampus tmc
								    inner join public.naturezadespesa nd on tmc.ndpid = nd.ndpid
								    inner join entidade.entidade e on e.entid = tmc.entid
								where tmc.tmcid = {$tmcid}";
						
			            $arDados = $db->carregar( $sql );
					}
			            $arDados = ($arDados) ? $arDados : array();
					?>
					<tbody id="tbodyTermoCampus">
					<?php if($arDados): ?>
						<?php $count = 1;?>
						<?php foreach ($arDados as $dados): ?>
						<tr class="trTermoCampus" id="<?php echo $count;?>">
							<td><?php echo $dados['desccampus']; ?></td>
							<td><?php echo $dados['tcaobjeto']; ?></td>
							<td><?php echo $dados['descnatureza']; ?></td>
							<td style="text-align: right" ><?php echo number_format($dados['tcavalor'],2,",","."); ?></td>
							<td style="text-align: center; width: 50px">
								<?php if($habilitadoTermoOrc == 'S'): ?>
									<img src="/imagens/check_p.gif" border=0 title="Alterar" style="cursor: pointer;" id="<?php echo $dados['tcaid']; ?>" class="alterarTermoCampus">&nbsp;<img src="/imagens/exclui_p.gif" id="<?php echo $dados['tcaid']; ?>" style="cursor: pointer;" border=0 title="Excluir" class="excluirTermoCampus">
								<?php else:?>
									<img src="/imagens/check_p_01.gif" border=0 title="Alterar" />&nbsp;<img src="/imagens/exclui_p2.gif" />
								<?php endif;?>
							</td>
						</tr>
						<?php 
							$somatcavalor = $somatcavalor + $dados['tcavalor'];
							$count++;
						?>
						<?php endforeach;?>
					<?php endif;?>
						<tr bgcolor="#ffffff" id="totalTermoCampus">
					    	<td colspan="3" style="background-color: #ffffff; text-align: right"><strong>TOTAL:</strong></td>
					    	<td style="background-color: #ffffff; text-align: right" id="tdTotal"><strong><?php echo number_format($somatcavalor,2,",","."); ?></strong></td>
					    	<td style="background-color: #ffffff;">&nbsp;</td>
					    </tr>
					</tbody>
				</table>
			</form>
		</td>
	</tr>
</table>
<?} ?>
<div id="divDebug"></div>
<!-- 
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr	class="SubTituloEsquerda" style="background-color: #cccccc">
		<td><strong>CRONOGRAMA DE DESEMBOLSO (EM 1,00)</strong></td>
	</tr>
	<tr>
		<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
			<form method="post" name="formulario" id="formulario">
				<input type="hidden" name="pesquisa" value="1"/>
				<table cellpadding="3" cellspacing="0" border="1" style="width: 100%">
					<tr>
						<td valign="top"><strong>N° da Parcela</strong></td>
						<td valign="top"><strong>Ação</strong></td>
						<td valign="top"><strong>Mês de liberação</strong></td>
						<td valign="top"><strong>Valor</strong></td>
						<td valign="top"><strong>Período de execução</strong></td>
					</tr>
					<tr bgcolor="#FFFFFF">
						<td valign="top">a</td>
						<td valign="top">a</td>
						<td valign="top">a</td>
						<td valign="top">a</td>
						<td valign="top">a</td>
					</tr>
				</table>
			</form>
		</td>
	</tr>
</table>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr	class="SubTituloEsquerda" style="background-color: #cccccc">
		<td><strong>CRONOGRAMA DE DESEMBOLSO (EM 1,00)</strong></td>
	</tr>
	<tr>
		<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
			<form method="post" name="formulario" id="formulario">
				<input type="hidden" name="pesquisa" value="1"/>
				<table cellpadding="3" cellspacing="0" border="1" style="width: 100%">
					<tr>
						<td valign="top"><strong>N° da Parcela</strong></td>
						<td valign="top"><strong>Ação</strong></td>
						<td valign="top"><strong>Mês de liberação</strong></td>
						<td valign="top"><strong>Valor</strong></td>
						<td valign="top"><strong>Período de execução</strong></td>
					</tr>
					<tr bgcolor="#FFFFFF">
						<td valign="top">a</td>
						<td valign="top">a</td>
						<td valign="top">a</td>
						<td valign="top">a</td>
						<td valign="top">a</td>
					</tr>
				</table>
			</form>
		</td>
	</tr>
</table>
-->
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr bgcolor="#cccccc">
		<td width="9px"></td>
		<td><input type="button" class="botao" value="Voltar"
			onclick="window.location.href = 'academico.php?modulo=principal/listaTermoCooperacao&acao=A'"></td>
	</tr>
</table>
<!-- <script>document.getElementById('tmcobs').value = document.getElementById('imitexto').value;</script>-->
