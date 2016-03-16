<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

unset($_SESSION['solid']);

if ($_GET['solid']){
	session_start();
	$_SESSION['solid'] = $_GET['solid'];
	header( "Location: atendefnde.php?modulo=principal/dadosSolicitacao&acao=A" );
	exit();
}


if ($_POST['ajaxlatid']){
	header('content-type: text/html; charset=ISO-8859-1');	
	
	$sql = "SELECT 
					 r.rsoid AS codigo,
					 u.usunome AS descricao
					FROM 
					 	atendefnde.responsavelsolicitacao r
						inner join seguranca.usuario u on u.usucpf = r.usucpf
					WHERE
					 r.latid = ".$_POST['ajaxlatid']." 
					ORDER BY 2";
	if($_POST['ajaxlatid'] != '999999'){
		$db->monta_combo("rsoid",$sql,"S","Selecione...","","","","","N","","");
	}
	else{
		$db->monta_combo("rsoid",array(),"N","Selecione o Setor","","","","","N");
	}
	
	exit;
}



if($_POST['consultaRapida'] == "ematendimento"){
	$_POST['numero'] 	= false;
	$numero   =  false;
	$ematendimento = true;
}
elseif($_POST['consultaRapida'] == "finalizada"){
	$_POST['numero'] 	= false;
	$numero   =  false;
	$finalizada = true;
}elseif(!$_POST['consultaRapida']){
	extract($_POST);
}




include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$menuAba = array(0 => array("id" => 1, "descricao" => "Lista de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/listaAtendimento&acao=A"),
			  1 => array("id" => 2, "descricao" => "Dados de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/dadosAtendimento&acao=A"),
			  2 => array("id" => 3, "descricao" => "Anexos de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/anexosAtendimento&acao=A"),
			  3 => array("id" => 4, "descricao" => "Lista de Solicitações", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/listaSolicitacao&acao=A"),
			  4 => array("id" => 5, "descricao" => "Cadastro de Solicitação", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/cadastraSolicitacao&acao=A")
		  	  );

echo montarAbasArray($menuAba, $_SERVER['REQUEST_URI']);

$titulo_modulo = "Lista de Solicitações";
monta_titulo($titulo_modulo, '&nbsp;');



// Atribui as variáveis $_POST para preenchimento dos campos.
$numero 							= (isset($_POST["numero"]) && $_POST["numero"]!="") ? $_POST["numero"] : NULL;
$assunto							= (isset($_POST["assunto"]) && $_POST["assunto"]!="") ? $_POST["assunto"] : NULL;
$latid 								= (isset($_POST["latid"]) && $_POST["latid"]!="") ? $_POST["latid"] : NULL;
$rsoid 								= (isset($_POST["rsoid"]) && $_POST["rsoid"]!="") ? $_POST["rsoid"] : NULL;


?>
<html>
	<head>
	
			 <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
			 <script src="../includes/calendario.js"></script>
			 <script type="text/javascript" src="../includes/funcoes.js"></script>
			 
			 <script type="text/javascript" src="/includes/prototype.js"></script>
			 <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
			 
	
	<script type="text/javascript">


	function buscaCodigo(e){

		var key; 
		if(e && e.which){
			key = e.which;
		}
		else if(window.event){
			e = window.event;
			key = e.keyCode;
		}

		if(key == 13 && document.formulario.codigo.value!='') document.formulario.submit();
	}

	function filtraResp(latid){
		if(!latid) latid = '999999';

		var div = document.getElementById('div_resp');  
		 
		// Faz uma requisição ajax, passando o parametro 'uf', via POST
		var req = new Ajax.Request('atendefnde.php?modulo=principal/listaSolicitacao&acao=A', {
				        method:     'post',
				        parameters: '&ajaxlatid='+latid,
				        onComplete: function (res)
				        {	
							div.innerHTML = res.responseText;
							//div.style.display = "";
				        }
				  });

	
	}

	</script>
	</head>
<body>
<form action="" method="POST" name="formulario">
	<input type="hidden" name="submetido" value="1" />
	
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="0">
		<tr>
			<td style="background-color:#e9e9e9; color:#404040;" >
			  <table align="center" border="0"  cellpadding="0" cellspacing="0" >
			  	<tr>
			  		<td style="background-color:#e9e9e9; color:#404040;">
			  		
						<table border="0"  cellpadding="3" cellspacing="0">
							<tr>
								<td width="10%" valign="top">
									<B>Número:</B>
									<br>
									<?= campo_texto( 'numero', 'N', 'S', '', 10, 15, '##########', '', '','','','id="numero"','buscaCodigo(event)' ); ?>
								</td>							
							</tr>
							<tr >
								<td colspan="3">
									<B>Assunto / Descrição:</B>
									<br>
									<?= campo_texto( 'assunto', 'N', 'S', '', 30, 60, '', '', '', '', '', 'id="assunto"', '', ''); ?>
								</td>
							</tr>
							<tr >
								<td colspan="3">
									<B>Setor:</B>
									<br>
									<?php $sql = "SELECT
														latid AS codigo,
														latdescricao AS descricao
													FROM
														atendefnde.localatendimento
													WHERE
														latstatus = 'A'
													ORDER BY 2 ASC"; 

										$db->monta_combo("latid",$sql,"S","Selecione...","filtraResp","","","","N","","",$_POST['latid']);
									?>
								</td>
							</tr>
							<tr >
								<td colspan="3">
									<B>Responsável:</B>
									<br>
									<div id="div_resp">
										<?php if($_POST['latid']): ?>
											<?php $sql = " SELECT 
															 r.rsoid AS codigo,
															 u.usunome AS descricao
															FROM 
															 	atendefnde.responsavelsolicitacao r
																inner join seguranca.usuario u on u.usucpf = r.usucpf
															WHERE
															 r.latid = {$_POST['latid']} 
															ORDER BY 2";
											?>
											<?php $db->monta_combo("rsoid",$sql,"S","Selecione...","","","","","N","","",$_POST['rsoid']) ?>
										<?php else: ?>
											<?php $db->monta_combo("rsoid",array(),"N","Selecione o Setor","","","","","N") ?>
										<?php endif; ?>
									</div>
								</td>
							</tr>
							</table>

						<div style="float: left;margin-top:15px;">	
							<input style="cursor:pointer;" type="button" onclick="submeteDados();" name="pesquisar" value="Pesquisar"/>
							<input style="cursor:pointer;" type="button" onclick="limparDados();" name="limpar" value="Limpar"/>
						</div>
						
						
					</td>
				</tr>
				</table>

						<a href="javascript:window.location.href='atendefnde.php?modulo=principal/cadastraSolicitacao&acao=A'"> <img src="../imagens/gif_inclui.gif" class="link img_middle"  > <b>Incluir Solicitação</b></a>			  		
			  		
			  		</td>
			  		<td style="background-color:#e9e9e9; color:#404040;">
			  		
						<div style="float:center;position:relative;width:240px;">
							<fieldset style="background-color:#ffffff;">
							<legend> <img id="mais_consulta1" style="cursor:pointer;display:none" onclick="exibeConsultaRapida1();" src="../imagens/mais.gif" /> <img  style="cursor:pointer" id="menos_consulta1" onclick="escondeConsultaRapida1();" src="../imagens/menos.gif" /> <b>Consulta por Situação</b></legend>
							<div style="width:230px"></div>
							<table id="consultas_rapidas1" width="100%" >
								<tr>
									<td valign="bottom" height="50%" style="padding-right:15px;">
										<a style="cursor:pointer;<?($ematendimento)? print "font-weight:bold;" : ""; ?>" title="Listar Solicitações Em Atendimento." onclick="selecionaEmAtendimento();demandasEmAtendimento();">Em Atendimento<span id="numero_ematendimento"></span></a>
										<input style="display:none" type="radio" id="ematendimento" name="consultaRapida" value="ematendimento" <?=$emtendimento ? 'checked' : ''?>>
									</td>
									<td valign="bottom">
										<a style="cursor:pointer;<?($finalizada)? print "font-weight:bold;" : ""; ?>" onclick="selecionaFinalizada();demandasFinalizada();"  title="Listar Solicitações Finalizadas.">Finalizada<span id="numero_finalizada"></span></a>
										<input style="display:none" type="radio" id="finalizada" name="consultaRapida" value="finalizada" <?=$finalizada ? 'checked' : ''?>>
									</td>
								</tr>
							</table>
							</fieldset>
		
						</div>
			  		
			  		
			  		
			  		
			  		</td>
			  	</tr>
			  
			  </table>	
			  
			 	
</form>
<?php
if($_POST["submetido"] == '1') {

	$whereFiltro = "";
	
	if($_REQUEST['numero']){
		$whereFiltro .= " AND s.solid = {$_REQUEST['numero']} ";

	}
	if($_REQUEST['assunto']){
		$whereFiltro .= " AND (s.solassunto ILIKE '%{$_REQUEST['assunto']}%' OR s.soldescricao ILIKE '%{$_REQUEST['assunto']}%') ";

	}
	if($_REQUEST['latid']){
		$whereFiltro .= " AND r.latid = {$_REQUEST['latid']} ";

	}
	if($_REQUEST['rsoid']){
		$whereFiltro .= " AND r.rsoid = {$_REQUEST['rsoid']} ";

	}
	
}
	$sql = "SELECT distinct
					'<img src=\"/imagens/alterar.gif\" border=0 title=\"Editar Solicitação\" style=\"cursor:pointer;\" onclick=\"window.location=\'atendefnde.php?modulo=principal/listaSolicitacao&acao=A&solid='||s.solid||'\'\">' as acao,
					s.solid,
					s.solassunto as assunto,
					u.usunome as responsavel,
					'Cadastrado' as situacao
			FROM atendefnde.solicitacao s
			INNER JOIN atendefnde.responsavelsolicitacao r on r.rsoid = s.rsoid
			INNER JOIN seguranca.usuario u on u.usucpf = r.usucpf
			WHERE s.ateid = ".$_SESSION['ateid']."
			$whereFiltro
			ORDER BY 2 ASC";
	//dbg($sql);
	$cabecalho = array("Ação", "Número", "Assunto", "Responsável", "Situação");
	$tamanho       = array("5%","5%","20%","20%","10%");
	$alinhamento   = array("center","center","left","left","center");
	$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2,"",$tamanho,$alinhamento);

	
//}else{
	?>
	<!-- 
	<table class="listagem" width="95%" align="center">
		<tr>
			<td align="center">
				<font color="red"><b>Escolha seu filtro e clique no botão pesquisar!<b/></font>
			</td>
		</tr>
	</table>
	 -->
<?//}?>
</body>
</html>

<script type="text/javascript">
	//varial global
	d = document;

	function submeteDados() {

		
		formulario.submit();
	}

	function limparDados() {


		location.href='atendefnde.php?modulo=principal/listaSolicitacao&acao=A';
		
	}	


function demandasEmAtendimento(){
	//d.getElementById('dataini').value = '';
	//d.getElementById('datafim').value = '';
	document.formulario.submit();
}

function demandasFinalizada(){
	//d.getElementById('dataini').value = '';
	//d.getElementById('datafim').value = '';
	document.formulario.submit();
}

function exibeConsultaRapida1(){
	d.getElementById('consultas_rapidas1').style.display = "";
	d.getElementById('menos_consulta1').style.display = "";
	d.getElementById('mais_consulta1').style.display = "none";
}




function escondeConsultaRapida1(){
	d.getElementById('consultas_rapidas1').style.display = "none";
	d.getElementById('mais_consulta1').style.display = "";
	d.getElementById('menos_consulta1').style.display = "none";
}



function limpaCampos(){
	d.getElementById('numero').value = "";
	d.getElementById('assunto').value = "";
	d.getElementsByName('latid').value = '';
	d.getElementsByName('rsoid').value = '';
}


function limpaConsultaRapida(){
	d.getElementById('ematendimento').checked = false;
	d.getElementById('finalizada').checked = false;
}

function selecionaFinalizada(){
	limpaCampos();
	d.getElementById('finalizada').checked = true;
	d.getElementById('ematendimento').checked = false;
}

function selecionaEmAtendimento(){
	limpaCampos();
	d.getElementById('finalizada').checked = false;
	d.getElementById('ematendimento').checked = true;
}

</script>
