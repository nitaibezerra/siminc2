<?php
include APPRAIZ ."includes/workflow.php";
include_once "_funcoes_proinfancia.php";

$obrid = $_SESSION['proinfantil']['obrid'];
$pinid = $_SESSION['proinfantil']['pinid'];

$perfis = pegaPerfil($_SESSION['usucpf']);

if($_POST['requisicao'] == 'parecerdiligencia'){
	
	$arDados = array('docid' => $_POST['docid']);
		
	$sql = "SELECT esdid FROM workflow.documento WHERE docid = {$_POST['docid']}";								
	$esdid 			= $db->pegaUm( $sql );
	$esdiddestino 	= WF_PROINFANTIL_EM_DILIGENCIA;
	
	$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
	$aedid = $db->pegaUm($sql);
	
	wf_alterarEstado( $_POST['docid'], $aedid, str_ireplace('\"', '"', $_POST['parecerdiligencia']), $arDados );
	$db->sucesso('principal/analise');	
	exit();
}

if($_POST['requisicao'] == 'parecerindeferido'){
	
	$arDados = array('docid' => $_POST['docid']);
		
	$sql = "SELECT esdid FROM workflow.documento WHERE docid = {$_POST['docid']}";								
	$esdid 			= $db->pegaUm( $sql );
	$esdiddestino 	= WF_PROINFANTIL_OBRA_ARQUIVADA;
	
	$sql = "select aedid from workflow.acaoestadodoc where esdidorigem = $esdid and esdiddestino = ".$esdiddestino;
	$aedid = $db->pegaUm($sql);
	
	wf_alterarEstado( $_POST['docid'], $aedid, str_ireplace('\"', '"', $_POST['parecerindeferido']), $arDados );
	$db->sucesso('principal/analise');	
	exit();
}
if($_POST['historico']){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql_historico = "SELECT 		u.usunome, p.prapareceraprovacao, to_char(p.pradata,'DD/MM/YYYY'), CASE WHEN p.prastatus = 'A' THEN 'Ativo' ELSE 'Inativo' END prastatus 
					  FROM 			proinfantil.proinfanciaanalise p
					  LEFT JOIN 	seguranca.usuario u ON u.usucpf = p.usucpf 
					  WHERE 		p.praanoanalise = '1' AND p.pinid = {$_SESSION['proinfantil']['pinid']}
					  ORDER BY 		p.praid DESC";
	
	$historico = $db->carregar($sql_historico);
	$cab = array('Analista','Parecer','Data','Status');
	$db->monta_lista($sql_historico, $cab, 1000, 10, 'N', '', '', '', '', ''); 
	exit();
}

if( !in_array(PERFIL_SUPER_USUARIO,$perfis) && !in_array(PERFIL_COORDENADOR,$perfis) && !in_array(PERFIL_ANALISTA,$perfis) && !in_array(PERFIL_ADMINISTRADOR,$perfis) && !in_array(CONSULTA_GERAL,$perfis)){
		print "<script>alert('Seu perfil não possui acesso a esta página!');</script>";
		print "<script>history.back();</script>";
		exit;	
}

if($_REQUEST['requisicao']){
	if($_REQUEST['requisicao'] == '1'){ //grava campo total mes
		switch (substr($_REQUEST['dataini'],6,10)){
			case '2011': $datacenso = '20110525';
				break;
			case '2012': $datacenso = '20120530';
			  	break;
			case '2013': $datacenso = '20130529';
			  	break;
			case '2014': $datacenso = '20140528';
			 	break;
			case '2015': $datacenso = '20150527';
			  	break;
			case '2016': $datacenso = '20160525';
			 	break;
			case '2017': $datacenso = '20170531';
			  	break;
			case '2018': $datacenso = '20180530';
				break;
			case '2019': $datacenso = '20190529';
				  break;
			case '2020': $datacenso = '20200527';
				  break;
		}
		
		$anoatend = substr($_REQUEST['dataini'], 6, 4);
		$anoenvio = substr($_REQUEST['dataana'], 6, 4);
		
		$mescenso = '5';
		$qtdlimite = 0;
		if(strtotime(str_replace("-","",formata_data_sql($_REQUEST['dataini']))) > strtotime($datacenso)){
			$qtdlimite = '18';
		} elseif(strtotime(str_replace("-","",formata_data_sql($_REQUEST['dataini']))) <= strtotime($datacenso)){
			$qtdlimite = '12';
		}
				
		if($qtdlimite == '12'){
			if( $anoenvio > $anoatend){
				$saldolimite = 0;
			} else {
				$saldolimite = (12 - (substr($_REQUEST['dataana'],3,2) - 1));
			}	
		}

		if($qtdlimite == '18'){
			if( $anoenvio > ((int)$anoatend+1) ){
				//$saldolimite = 0;
				$saldolimite = (12 - (substr($_REQUEST['dataana'],3,2) - 1));
			} else {
				$saldolimite = (12 - (substr($_REQUEST['dataana'],3,2) - 1))+ 12;
			}
		}
		
		$mesatend = substr($_REQUEST['dataini'],3,2);
		$mesenvio = substr($_REQUEST['dataana'],3,2);
		$mesAtraso = (int)$mesenvio - (int)$mesatend;
		
		if( (int)$qtdlimite < (int)$saldolimite ){
			$saldolimite = (int)$qtdlimite;
		}
		
		if($_POST['pinperiodorepasse'] < 0 || $_POST['pinperiodorepasse'] > $saldolimite){
			echo "<script type='text/javascript'>
						alert('Período informado não permitido!');
						location.href='proinfantil.php?modulo=principal/analise&acao=A'</script>";	
			exit();
		}
		
		$sql = "UPDATE 		proinfantil.proinfantil 
				SET 		pinperiodorepasse = '{$_POST['pinperiodorepasse']}'
				WHERE 		pinid = {$pinid}";
		$db->executar($sql);					
		$db->commit();
		
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>location.href='proinfantil.php?modulo=principal/analise&acao=A'</script>";	
		
	} elseif($_REQUEST['requisicao'] == '2'){ //grava campo textarea

		$sql = "UPDATE 		proinfantil.proinfantil 
				SET 		pinpareceraprovacao = '{$_POST['pinpareceraprovacao']}',
							pinanoseguinte = ".($_POST['pinanoseguinte'] ? "'".$_POST['pinanoseguinte']."'" : "null")."
				WHERE 		pinid = {$pinid}";
		$db->executar($sql);					
		$db->commit();
		
		//Adicionado para alimentar histórico de pareceres
		$sql = "SELECT 	MAX(praid) as praid  
  				FROM 	proinfantil.proinfanciaanalise
  				WHERE 	pinid = {$pinid} AND prastatus = 'A' AND praanoanalise = 1";
		$res = $db->pegaLinha($sql);
	
		if(empty($res['praid'])){
			
			$sql_ins = "INSERT INTO proinfantil.proinfanciaanalise
					 (prapareceraprovacao, prastatus, pradata, usucpf,pinid,praanoanalise)
					 VALUES
					 ('{$_POST['pinpareceraprovacao']}','A','NOW()','{$_SESSION['usucpf']}',{$pinid},'1')";
			$db->executar($sql_ins);
			$db->commit();
		} else {
			$sql_up = "UPDATE proinfantil.proinfanciaanalise 
					   SET	  prastatus = 'I' 
					   WHERE  praid = {$res['praid']}";
			$db->executar($sql_up);
			$sql_in = "INSERT INTO proinfantil.proinfanciaanalise
					 (prapareceraprovacao, prastatus, pradata, usucpf, pinid, praanoanalise)
					 VALUES
					 ('{$_POST['pinpareceraprovacao']}','A','NOW()','{$_SESSION['usucpf']}',{$pinid},'1')";
			$db->executar($sql_in);
			$db->commit();
		} 
		
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>location.href='proinfantil.php?modulo=principal/analise&acao=A'</script>";	
		
	} elseif($_REQUEST['requisicao'] == '3'){ //gerar termo
		
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
		
		//excluir arquivo
		$arqid = $db->pegaUm("SELECT arqid FROM proinfantil.termoadesao WHERE teatipoano = '1' AND pinid = {$pinid}");
		if($arqid){
			$file = new FilesSimec("termoadesao", NULL ,"proinfantil");
			$file->excluiArquivoFisico($arqid);
			
			$sql = "DELETE FROM proinfantil.termoadesao	WHERE teatipoano = '1' AND pinid = {$pinid}";
			$db->executar($sql);
			unset($arqid);
		}	
	    
		//inserir arquivo
		$campos = array("pinid"         	=> "'".$pinid."'",
						"usucpf"        	=> "'".$_SESSION['usucpf']."'",
						"teadatainclusao" 	=> "NOW()",
						"teatipoano" 		=> "'1'",
						"teaadesao"    	 	=> 'true');
	
		$arParam = array( 'tabela' => 'termoadesao',
						  'campo' => $campos,
						  'extensao' => 'pdf',
						  'nome' => "a1_termo$pinid"
					 );
					 
		$html = geraTermoHtml($obrid, $pinid, '1');
		$html = utf8_encode($html);
		$http = new RequestHttp();
		$http->toPdfSave( $html, $arParam );
		$db->commit();
		
		print "<script>alert('Operação realizada com sucesso!');</script>";
		print "<script>opener.location.href='proinfantil.php?modulo=principal/analise&acao=A'</script>";
		print "<script>location.href='proinfantil.php?modulo=principal/analise&acao=A&requisicao=4'</script>";
		
	} elseif($_REQUEST['requisicao'] == '4'){
		
		$arqid = $db->pegaUm("SELECT arqid FROM proinfantil.termoadesao WHERE teatipoano = '1' AND pinid = {$pinid}");
		
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		ob_clean();
		$file = new FilesSimec("termoadesao", NULL ,"proinfantil");
		$file->getDownloadArquivo($arqid);
		
	}
}

if( empty($_SESSION['proinfantil']['popup']) ){
	include APPRAIZ . "includes/cabecalho.inc";
	echo '<br/>';
} else {
	echo '<script type="text/javascript" src="/includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
}

montaAbasProinfantil( $abacod_tela, $url );

monta_titulo('Período para Cálculo do Repasse dos Recursos', '');
cabecalhoProInfantil($obrid);
$habilitado = verificaLiberacao();

//Carrega Campos
$sql = "SELECT 		pi.pinperiodorepasse, pi.pinanoseguinte, t.teaid
		FROM 		proinfantil.proinfantil pi
		LEFT JOIN 	proinfantil.termoadesao t ON t.pinid = pi.pinid AND t.teatipoano = '1'
		WHERE 		pi.pinid = {$pinid}";
$dados = $db->pegaLinha($sql);
if($dados) extract($dados);

//Carrega Parecer
$sql_parecer = "SELECT 	prapareceraprovacao as pinpareceraprovacao
				FROM 	proinfantil.proinfanciaanalise 
				WHERE 	pinid = {$pinid} AND prastatus = 'A' AND praanoanalise = 1";

$dados_parecer = $db->pegaLinha($sql_parecer);
if($dados_parecer) extract($dados_parecer);

$docid = recuperaDocumentoProInfantil($pinid);
$esdid = pegaEstadoAtualDocumento($docid);
?>

<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle;border:0px}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
	.red{color:#990000}
	.div_historico { background: #white; }
	.div_historico:hover { background: #eeeeaa; }
</style>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script> 
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

<script type="text/javascript">
//jQuery.noConflict();

function valida1(){
	var dataini = $("#dataini").val();
	var dataana = $("#dataana").val();
	var vl = $("#pinperiodorepasse").val();
	var qtdlimite = 0;
	var saldolimite = 0;
	
	var anoatend = parseInt(dataini.substring(6,10));
	var anoenvio = parseInt(dataana.substring(6,10));
	
	if(dataini) dataini = dataini.substring(6,10) + dataini.substring(3,5) + dataini.substring(0,2);
	if(dataana) dataana = dataana.substring(6,10) + dataana.substring(3,5) + dataana.substring(0,2);
	
	var datacenso = '';
	var mescenso  = '05';
	var mesatend = parseInt(dataini.substring(4,6));
	var mesenvio = parseInt(dataana.substring(4,6));
	
	var mesAtraso = parseInt(mesenvio) - parseInt(mesatend);
	
	switch (dataini.substring(0,4)){
		case '2011': datacenso = '20110525';
			break;
		case '2012': datacenso = '20120530';
		  	break;
		case '2013': datacenso = '20130529';
		  	break;
		case '2014': datacenso = '20140528';
		 	break;
		case '2015': datacenso = '20150527';
		  	break;
		case '2016': datacenso = '20160525';
		 	break;
		case '2017': datacenso = '20170531';
		  	break;
		case '2018': datacenso = '20180530';
			break;
		case '2019': datacenso = '20190529';
			  break;
		case '2020': datacenso = '20200527';
			  break;
	} 
	
	if(parseFloat(dataini) > parseFloat(datacenso)){
		qtdlimite = 18;
	} else if(parseFloat(dataini) <= parseFloat(datacenso)){
		qtdlimite = 12;
	} 	
	
	if(qtdlimite == 12){
		if( anoenvio > anoatend ){
			saldolimite = 0;
		} else {
			saldolimite = (12 - (parseInt(dataana.substring(4,6)) - 1));
		}
	}

	if(qtdlimite == 18){
		if( anoenvio > anoatend ){
			////saldolimite = 0;
			saldolimite = (12 - (parseInt(dataana.substring(4,6)) - 1));
		} else {
			saldolimite = (12 - (parseInt(dataana.substring(4,6)) - 1)) + 12;
		}
	}
	
	//#De acordo com a resolução nº 28/2012, quem iniciar turmas em nov e dez só receberá no próximo ano
	if( mesatend == 11 || mesatend == 12 ){
		if( mesenvio == 11 || mesenvio == 12 ){
			saldolimite = 12;
		} else {
			saldolimite = ( (12 - mesenvio) + 1);
		}
	} 
	
	if( qtdlimite < saldolimite ){
		saldolimite = qtdlimite;
	}

	if(vl == '' || parseInt(vl) < 0 || parseInt(vl) > saldolimite){
		alert ("O campo 'Período considerado para o repasse de recursos (Total de meses)' é inválido.\n\nFavor informar o valor entre 0 a "+saldolimite+".");
		$("#pinperiodorepasse").focus();
		return false;
	} 
	
	/*if(vl == ''){
		alert ("O campo 'Período considerado para o repasse de recursos (Total de meses)' deve ser preenchido.");
		$("#pinperiodorepasse").focus();
		return false;
	}*/

	document.formulario.requisicao.value='1';
	document.formulario.submit();
}

function valida2(){
	if($("#pinpareceraprovacao").val() == ''){
		alert ("O campo 'Parecer de Aprovação' deve ser preenchido.");
		$("#pinpareceraprovacao").focus();
		return false;
	}
	document.formulario.requisicao.value='2';
	document.formulario.submit();
}


function gerarParecer(tipo){
	document.formulario.requisicao.value=tipo;
	if(tipo == 3){
		document.formulario.action = 'proinfantil.php?modulo=principal/analise&acao=A';
		window.open( '', 'geraTermo', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		document.formulario.target = 'geraTermo';
	}
	document.formulario.submit();
}

$(function(){
	function aguarda(val){
		if(val){
			$('#aguardando').show();
		}else{
			$('#aguardando').hide();
		}
	}
	$('.div_historico').click(function(){
		aguarda(true);
		var id = $(this).attr('id');
		if($('#td_'+id).html() != ''){
			if( $('#td_'+id).css('display') == 'none' ){
				$('#td_'+id).show();
				$('#imgmais'+id).hide();
				$('#imgmenos'+id).show();
			} else {
				$('#td_'+id).hide();
				$('#imgmais'+id).show();
				$('#imgmenos'+id).hide();
			}
			aguarda(false);
		} else {
			$.ajax({
				type: "POST",
				url: 'proinfantil.php?modulo=principal/analise&acao=A',
				data: "historico=true&id="+id,
				async: false,
				success: function(msg){
					$('#td_'+id).html(msg);
					$('#td_'+id).show();
					$('#imgmais'+id).hide();
					$('#imgmenos'+id).show();					
					aguarda(false);
				}
			});
		}
	});
	aguarda(false);
});

function popGuiaParecer(modid,campoTipo,campoParecer){
	w = window.open('?modulo=principal/popGuiaParecer&acao=A&filtraTipo=1&modid='+modid+'&tipid=1&campoTipo='+campoTipo+'&campoParecer='+campoParecer,'guiaparecer','scrollbars=yes,location=no,toolbar=no,menubar=no,width=550,height=200,left=250,top=125'); 
	w.focus();	
}
function carregarParcerAnalise( parecer, tipid, campoTipo, campoParecer ){
	$('[name="'+campoParecer+'"]').val(parecer);
	$('[name="'+campoTipo+'"]').val(tipid);	
}

function wfalterarEstado(){
		
	$( "#dialog_obra" ).show();
	$( '#dialog_obra' ).dialog({
			resizable: false,
			width: 900,
			modal: true,
			position: {  my: "center", at: "center", of: "#dialog_obra" },
			show: { effect: 'drop', direction: "up" },
			buttons: {
				'Salvar Parecer': function() {
					if( $('[name="parecerdiligencia"]').val() == '' ){
						alert('Informe um parecer');
						return false;
					} else {
						$('[name="requisicao"]').val('parecerdiligencia');
						$('[name="formobraCad"]').submit();
					}
				},
				'Cancelar': function() {
					$( this ).dialog( 'close' );
				}
			}
	});
}

function wfalterarEstadoIndeferido(){
		
	$( "#dialog_indeferido" ).show();
	$( '#dialog_indeferido' ).dialog({
			resizable: false,
			width: 900,
			modal: true,
			position: {  my: "center", at: "center", of: "#dialog_indeferido" },
			show: { effect: 'drop', direction: "up" },
			buttons: {
				'Salvar Parecer': function() {
					if( $('[name="parecerindeferido"]').val() == '' ){
						alert('Informe um parecer');
						return false;
					} else {
						$('[name="requisicao"]').val('parecerindeferido');
						$('[name="formobraCad"]').submit();
					}
				},
				'Cancelar': function() {
					$( this ).dialog( 'close' );
				}
			}
	});
}

function guiaParecerNovasTurmas(modid, tipid, campo){
	w = window.open('?modulo=novasturmas/guiaParecerNovasTurmas&acao=A&modid='+modid+'&tipid='+tipid+'&campo='+campo,'guiaparecer','scrollbars=yes,location=no,toolbar=no,menubar=no,width=800,height=400,left=250,top=125'); 
	w.focus();	
}
</script>

<div id="dialog_obra" title="Parecer de Diligência" style="display: none; text-align: center;">
	<form name="formobraCad" id="formobraCad" action="" enctype="multipart/form-data" method="post">
		<input type="hidden" name="requisicao" id="requisicao" value="">
		<input type="hidden" name="docid" id="docid" value="<?=$docid; ?>">
		
		<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" style="width:100%;">
			<tr>
				<td class="subtituloDireita" style="text-align: center;">Parecer</td>
			</tr>
			<tr>
				<td><?=campo_textarea('parecerdiligencia', 'N', 'S', '', 135, 15, '', '', '', '', false, '', '', array('id'=>'parecerdiligencia')); ?></td>
			</tr>
			<tr>
				<td><a href="#" style="cursor:pointer;vertical-align: top" onclick="guiaParecerNovasTurmas('1', '2', 'parecerdiligencia')"><img src="../imagens/gif_inclui.gif"> Adicionar Parecer</a></td>
			</tr>
		</table>
	</form>
</div>

<div id="dialog_indeferido" title="Parecer de Indeferido/Arquivado" style="display: none; text-align: center;">
	<form name="formobraCad" id="formobraCad" action="" enctype="multipart/form-data" method="post">
		<input type="hidden" name="requisicao" id="requisicao" value="">
		<input type="hidden" name="docid" id="docid" value="<?=$docid; ?>">
		
		<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" style="width:100%;">
			<tr>
				<td class="subtituloDireita" style="text-align: center;">Parecer</td>
			</tr>
			<tr>
				<td><?=campo_textarea('parecerindeferido', 'N', 'S', '', 135, 15, '', '', '', '', false, '', '', array('id'=>'parecerindeferido')); ?></td>
			</tr>
			<tr>
				<td><a href="#" style="cursor:pointer;vertical-align: top" onclick="guiaParecerNovasTurmas('1', '3', 'parecerindeferido')"><img src="../imagens/gif_inclui.gif"> Adicionar Parecer</a></td>
			</tr>
		</table>
	</form>
</div>

<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>

<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" name="requisicao" value="">
	<input type="hidden" name="tipotermo" value="">
	<input type="hidden" value="<?php echo $pinid; ?>" name="pinid" />
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	        <td width="25%" class="SubTituloDireita">Data de início do atendimento às crianças:</td>
	        <td>
	        	<?php
	        		$dataini = recuperaDataInicioAtendimento($pinid);
	        		echo $dataini;
	        	?>
	        	<input id="dataini" type="hidden" name="dataini" value="<?php echo $dataini; ?>">
	        </td>
	    </tr>
		<tr>
	        <td width="25%" class="SubTituloDireita">Data de envio para a análise:</td>
	        <td>
	        	<?php
					$data_analise = recuperaDataEnvioAnalise($docid);
	        		echo $data_analise;
	        	?>
	        	<input id="dataana" type="hidden" name="dataana" value="<?php echo $data_analise; ?>">
	        </td>
	    </tr>	    
	    <tr>
	        <td class="SubTituloDireita">Período considerado para o repasse de recursos:<br>(Total de meses)</td>
	        <td>
	        	<?php
	        		echo campo_texto( 'pinperiodorepasse', 'N',$habilitado, '', 4, 2, '##', '', '','','','id="pinperiodorepasse"','' );
	        		echo " mês(es) ";
	        		echo obrigatorio(); 
	        	?>
	        </td>
	    </tr>
	    <tr>
	    	<td colspan="2">
				<table border="0" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
				    	<td align="left" style="font-size: 11px">
				    		(RESOLUÇÃO CD/FNDE Nº 52 DE 29 DE SETEMBRO DE 2011 - Art. 9º No ano de 2011, excepcionalmente,
				    		os estabelecimentos que iniciaram seu atendimento antes da publicação desta Resolução farão jus
				    		a, no máximo, 7/12 do valor aluno-ano definido pelo Fundeb para creche e pré-escola em período
				    		integral e parcial no exercício de 2010, conforme Portaria MEC 647, de 23 de maio de 2011.)
				    	</td>
				    </tr>
				</table>	    	
	    	</td>
	    </tr>
	    <?php if($habilitado == 'S' && empty($_SESSION['proinfantil']['popup']) ){?>
		<tr bgcolor="#C0C0C0">
			<td align="center" colspan="2"><input id="btnsalvar" type="button" class="botao" name="btnsalvar" value="Salvar" onclick="valida1();"></td>
		</tr>	    
		<?php } ?>
	</table>
	<?php $data_atendimento = formata_data_sql($dataini); ?>
	
	<?php $arrModalidades = recuperaTipoModalidade(); ?>
	<?php $arrAlunos = $pinid ? recuperaAlunoAtendido($pinid) : array(); ?>
	<?php $periodoTotalMeses = $pinperiodorepasse; ?>

	<?php if($periodoTotalMeses >= 0){?>	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width:95%">
	    <tr>
	    	<td>
	    		<br>
				<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr class="SubTitulotabela">
				    	<td colspan="5" class="center bold">VALOR DO REPASSE</td>
				    </tr>				    
				    <tr class="SubTitulotabela" >
				    	<td class="center bold">Etapa</td>
				    	<td class="center bold">Matrículas não Computadas <br>para Recebimento do Fundeb</td>
				    	<td class="center bold">Valor Unitário</td>
				    	<td class="center bold">Período Considerado para o Repasse<br> de Recursos (meses)</td>
				    	<td class="center bold">Valor Total</td>
				    </tr>
				    <?php if($arrModalidades): ?>

				    	<?php $somaColuna=0 ?>
				    	<?php $somaColuna4=0 ?>
				    	<?php $cor = "#FFFFFF" ?>
				    	
				    	<?php foreach($arrModalidades as $modalidade): ?>

							<?php $calculaLinha=0 ?>
						    <?php $cor = "#FFFFFF" ? "#f7f7f7" : "#FFFFFF" ?>
						    <?php $turno = 1;?>
						    <?php $vlPeriodo = $db->pegaUm("SELECT vaavalor as valor FROM proinfantil.valoraluno WHERE vaastatus = 'A' AND vaatipo = 'I' AND timid = {$modalidade['timid']} AND ('{$data_atendimento}' BETWEEN vaadatainicial AND vaadatafinal)");
						    	  $vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;
						    ?>
						    
						    <tr bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" >
						        <td><?php echo $modalidade['timdescricao'] ." Integral"?></td>
						        <td class="center bold" ><?php echo $arrAlunos[$turno][$modalidade['timid']] ? $arrAlunos[$turno][$modalidade['timid']] : '0';
						        								$somaColuna += $arrAlunos[$turno][$modalidade['timid']];
						        								$calculaLinha = $arrAlunos[$turno][$modalidade['timid']];
						        						 ?>
						        </td>
						        <td class="center bold" ><?php echo $vlPeriodo ? number_format($vlPeriodo,2,",",".") : '0,00';?>
						        </td>
						        <td class="center bold" ><?php echo $periodoTotalMeses;?> / 12</td>
						        <td class="center bold" ><?php $valorTotal = ($arrAlunos[$turno][$modalidade['timid']] * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $periodoTotalMeses);
						        								echo $valorTotal ? number_format($valorTotal,2,",",".") : '0,00';
						        								$somaColuna4 += $valorTotal;?>
						        </td>
						    </tr>
						    <?php $vlPeriodo = $db->pegaUm("SELECT vaavalor as valor FROM proinfantil.valoraluno WHERE vaastatus = 'A' AND vaatipo = 'P' AND timid = {$modalidade['timid']} AND ('{$data_atendimento}' BETWEEN vaadatainicial AND vaadatafinal)");
						    	  $vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;
						    ?>
							<?php $calculaLinha=0 ?>
						    <?php $cor = "#f7f7f7" ? "#FFFFFF" : "#f7f7f7" ?>
						    <?php $turno++;?>
						    
						    <tr bgcolor="<?php echo $cor ?>" onmouseout="this.bgColor='<?php echo $cor ?>';" onmouseover="this.bgColor='#ffffcc';" >
						        <td><?php echo $modalidade['timdescricao'] ." Parcial"?></td>
						        <td class="center bold" ><?php echo $arrAlunos[$turno][$modalidade['timid']];
						        								$somaColuna += $arrAlunos[$turno][$modalidade['timid']];
						        						 ?>
						        </td>
						        <td class="center bold" ><?php echo $vlPeriodo ? number_format($vlPeriodo,2,",",".") : '0,00';?>
						        </td>
						        <td class="center bold" ><?php echo $periodoTotalMeses;?> / 12</td>
						        <td class="center bold" ><?php $valorTotal = ($arrAlunos[$turno][$modalidade['timid']] * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $periodoTotalMeses);
						        								echo $valorTotal ? number_format($valorTotal,2,",",".") : '0,00';
						        								$somaColuna4 += $valorTotal;?>
						        </td>
						    </tr>						    
						    
					    <?php endforeach; ?>
					    <tr class="SubTitulotabela" >
					        <td class="bold" >Total Geral</td>
			        		<td class="center bold" ><?php echo $somaColuna ?></td>
					        <td class="center bold" >&nbsp;</td>
					        <td class="center bold" >&nbsp;</td>
					        <td class="center bold" ><?php echo $somaColuna4 ? number_format($somaColuna4,2,",",".") : '0,00';?></td>
					    </tr>
				    <?php endif; ?>				    
				</table>
				
				<br>
				
				<table border="0" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
				    	<td align="left" style="font-size: 11px">
				    		(RESOLUÇÃO CD/FNDE Nº 15 DE 16 DE MAIO DE 2013 - Art. 6º Parágrafo único. A referência para
				    		a base de cálculo será sempre o valor anual mínimo por matrícula em creche e em pré-escola,
				    		em período integral e parcial, estabelecido nacionalmente pelo Fundeb para o ano anterior,
				    		computando-se 1/12 desse valor para cada mês de funcionamento, disposto em portaria do
				    		Ministro de Estado da Educação.)
				    	</td>
				    </tr>
				</table>
				
				<br>
				
				<table border="0" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
				    	<td class="center bold" align="center" >
				    		PARECER DE APROVAÇÃO
				    		<br>
				    		<?php echo campo_textarea('pinpareceraprovacao', 'S', $habilitado, '', 150, 10, ''); ?>
				    		<input type="hidden" name="tipidParecer" id="tipidParecer" value="">
				    	</td>
				    	<?php if($habilitado == 'S' && empty($_SESSION['proinfantil']['popup']) ){ ?>
				    	<td align="left" valign="top">
				    		<br>
				    		<img src="../imagens/consultar.gif" style="cursor:pointer;vertical-align: top" onclick="popGuiaParecer('1','tipidParecer','pinpareceraprovacao')">
				    	</td>
				    	<?php } ?>
				    </tr>
				     <?php if($habilitado == 'S' && empty($_SESSION['proinfantil']['popup']) ){?>
				    <tr>
				    	<td colspan="2" align="center">
				    		<input id="btnsalvar2" type="button" class="botao" name="btnsalvar2" value="Salvar" onclick="valida2();">
				    	</td>
				    </tr>
				    <?php } ?>
				</table>

				<br>
	
				<?php 
				$sql_historico = "SELECT 		u.usunome, p.prapareceraprovacao, to_char(p.pradata,'DD/MM/YYYY'), CASE WHEN p.prastatus = 'A' THEN 'Ativo' ELSE 'Inativo' END prastatus 
								  FROM 			proinfantil.proinfanciaanalise p
								  LEFT JOIN 	seguranca.usuario u ON u.usucpf = p.usucpf 
								  WHERE 		p.praanoanalise = '1' AND p.pinid = {$_SESSION['proinfantil']['pinid']}
								  ORDER BY		p.praid DESC";
				$historico = $db->pegaLinha($sql_historico);
				if(!empty($historico)){?>
				<table border="0" class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" >
					<tr>
						<td style="background-color: #CDCDCD;">
							<div id="hist" class="div_historico" style="float:left;cursor:pointer;width:100%">
								<img src="../imagens/mais.gif" style="cursor:pointer; padding-left: 5px; padding-top: 5px;" id="imgmaishist">
								<img src="../imagens/menos.gif" style="cursor:pointer; display: none; padding-left: 5px; padding-top: 5px;" id="imgmenoshist">
								<center><b>Histórico de Pareceres</b></center>
							</div>
						</td>
					</tr>
					<tr>
						<td>
							<div id="td_hist" style="display:none;"></div>
						</td>
					</tr>	
				</table>
				<br>
				<?php } ?>				
			</td>
			<?if( empty($_SESSION['proinfantil']['popup']) ){ ?>
			<td width="80" valign="top"  align="right">
				<?php wf_desenhaBarraNavegacao( $docid , array( 'pinid' => $pinid ) ); ?>
				<br>
				<?php if(!in_array(CONSULTA_GERAL,$perfis)){ ?>
				<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
					<tr style="background-color: #c9c9c9; text-align: center;">
						<td style="font-size: 7pt; text-align: center;">
							<span title="estado atual"> <b>outras ações</b></span>
						</td>
					</tr>
					<?php if(!$teaid){ ?>
						<tr>
							<td style="font-size: 7pt; text-align: center; " onmouseover="this.style.backgroundColor='#ffffdd';" onmouseout="this.style.backgroundColor='';">
								<?php if(!$pinpareceraprovacao){ ?>
									<a href="javascript:alert('É necessário preencher o campo PARECER DE APROVAÇÃO.');">Gerar Parecer</a>
								<?php }else{ ?>
									<?php if($pinperiodorepasse < 0){ ?>
										<a href="javascript:alert('É necessário preencher o campo PERÍODO.');">Gerar Parecer</a>
									<?php } else { ?>
										<a href="javascript:gerarParecer(3);">Gerar Parecer</a>
									<?php } ?>
								<?php } ?>
							</td>
						</tr>
					<?php } else { ?>
						<?php if(in_array(PERFIL_SUPER_USUARIO,$perfis)){ ?>
							<tr>
								<td style="font-size: 7pt; text-align: center; " onmouseover="this.style.backgroundColor='#ffffdd';" onmouseout="this.style.backgroundColor='';">
									<?php if(!$pinpareceraprovacao){?>
										<a href="javascript:alert('É necessário preencher o campo PARECER DE APROVAÇÃO.');">Gerar Parecer</a>
									<?php }else{?>
										<a href="javascript:gerarParecer(3);">Gerar Parecer</a>
									<?php }?>
								</td>
							</tr>
						<?php } ?>
						<tr>
							<td style="font-size: 7pt; text-align: center; border-top: 2px solid #c9c9c9; " onmouseover="this.style.backgroundColor='#ffffdd';" onmouseout="this.style.backgroundColor='';">
								<a href="javascript:gerarParecer(4);">Emitir Parecer</a>
							</td>
						</tr>		
					<?php } ?>
					<?if($esdid == WF_PROINFANTIL_EM_ANALISE && (in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis )) ){ ?>
					<tr>
						<td onmouseout="this.style.backgroundColor='';" onmouseover="this.style.backgroundColor='#ffffdd';" style="font-size: 7pt; text-align: center; border-top: 2px solid rgb(208, 208, 208);">
							<a onclick="wfalterarEstado();" title="Devolver para correção" alt="Devolver para correção" href="#">
								<img border="0" align="absmiddle" src="../imagens/workflow/2.png"><br>Devolver para correção</a>
						</td>
					</tr>
					<tr>
						<td onmouseout="this.style.backgroundColor='';" onmouseover="this.style.backgroundColor='#ffffdd';" style="font-size: 7pt; text-align: center; border-top: 2px solid rgb(208, 208, 208);">
							<a onclick="wfalterarEstadoIndeferido();" title="Enviar para Indeferido Arquivado" alt="Enviar para Indeferido Arquivado" href="#">
								<img border="0" align="absmiddle" src="../imagens/workflow/2.png"><br>Enviar para Indeferido Arquivado</a>
						</td>
					</tr>
					<?} ?>													
				</table>
				<?php } ?>
			</td>
			<?} ?>
		</tr>
	</table>
	<?php } //fecha $periodoTotalMeses?>
</form>

<?php if($esdid == WF_PROINFANTIL_EM_CADASTRAMENTO){ ?>
<script type="text/javascript">
	$("#pinperiodorepasse").attr('disabled', 'disabled');
	$("#btnsalvar").attr('disabled', 'disabled');
	$("#pinpareceraprovacao").attr('disabled', 'disabled');
	$("#btnsalvar2").attr('disabled', 'disabled');
	$("#btnsalvar").hide();
	$("#btnsalvar2").hide();
</script>
<?php } ?>
