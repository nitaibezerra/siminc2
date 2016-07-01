<?php
header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past

unset($_SESSION['ateid']);

if ($_GET['ateid']){
	session_start();
	$_SESSION['ateid'] = $_GET['ateid'];
	header( "Location: atendefnde.php?modulo=principal/dadosAtendimento&acao=A" );
	exit();
}



//Ajax
if ($_POST['ufAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "	select
					muncod as codigo,
					mundescricao as descricao
				from
					territorios.municipio
				where
					estuf = '{$_POST['ufAjax']}'
				order by
					mundescricao";
	if($_REQUEST['ufAjax'] != 'XX'){
		$db->monta_combo("muncod",$sql,"S","Selecione...","","","","","N","","");
	}
	else{
		$db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","N");
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




//Ajax
if ($_REQUEST['dmdidAjax']) {
	header('content-type: text/html; charset=ISO-8859-1');
	montaSubLista($_POST['dmdidAjax']);
	exit;
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$menuAba = array(0 => array("id" => 1, "descricao" => "Lista de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/listaAtendimento&acao=A"),
			  1 => array("id" => 2, "descricao" => "Cadastro de Atendimento", 	"link" => "/atendefnde/atendefnde.php?modulo=principal/cadastraAtendimento&acao=A")
		  	  );

echo montarAbasArray($menuAba, $_SERVER['REQUEST_URI']);

$titulo_modulo = "Lista de Atendimento";
monta_titulo($titulo_modulo, '&nbsp;');



// Atribui as variáveis $_POST para preenchimento dos campos.
$numero 							= (isset($_POST["numero"]) && $_POST["numero"]!="") ? $_POST["numero"] : NULL;
$solicitante						= (isset($_POST["solicitante"]) && $_POST["solicitante"]!="") ? $_POST["solicitante"] : NULL;
$estuf 								= (isset($_POST["estuf"]) && $_POST["estuf"]!="") ? $_POST["estuf"] : NULL;
$muncod								= (isset($_POST["muncod"]) && $_POST["muncod"]!="") ? $_POST["muncod"] : NULL;
$dataini 							= (isset($_POST["dataini"]) && $_POST["dataini"]!="") ? $_POST["dataini"] : NULL;
$datafim 							= (isset($_POST["datafim"]) && $_POST["datafim"]!="") ? $_POST["datafim"] : NULL;



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

	function filtraMun(uf){
		if(!uf) uf = "XX";

		var div = document.getElementById('div_muncod');  
		 
		// Faz uma requisição ajax, passando o parametro 'uf', via POST
		var req = new Ajax.Request('atendefnde.php?modulo=principal/listaAtendimento&acao=A', {
				        method:     'post',
				        parameters: '&ufAjax=' + uf,
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
									<B>Solicitante:</B>
									<br>
									<?= campo_texto( 'solicitante', 'N', 'S', '', 30, 60, '', '', '', '', '', 'id="solicitante"', '', ''); ?>
								</td>
							</tr>
							<tr >
								<td colspan="3">
									<B>Estado:</B>
									<br>
									<?php $sql = "	select
														estuf as codigo,
														estdescricao as descricao
													from
														territorios.estado
													order by
														estdescricao"; 

										$db->monta_combo("estuf",$sql,"S","Selecione...","filtraMun","","","","N","","",$_POST['estuf']);
									?>
								</td>
							</tr>
							<tr >
								<td colspan="3">
									<B>Município:</B>
									<br>
									<div id="div_muncod">
										<?php if($_POST['estuf']): ?>
											<?php $sql = "	select
																muncod as codigo,
																mundescricao as descricao
															from
																territorios.municipio
															where
																estuf = '{$_POST['estuf']}'
															order by
																mundescricao" ?>
											<?php $db->monta_combo("muncod",$sql,"S","Selecione...","","","","","N","","",$_POST['muncod']) ?>
										<?php else: ?>
											<?php $db->monta_combo("muncod",array(),"N","Selecione o Estado","","","","","N") ?>
										<?php endif; ?>
									</div>
								</td>
							</tr>
							<tr>
								<td colspan="3">
									<B>Período:</B>
									<br>
									<?=campo_data('dataini', 'N', 'S', '', 'S' );?>
									&nbsp;&nbsp;&nbsp;&nbsp;
									até
									&nbsp;&nbsp;&nbsp;&nbsp;
									<?=campo_data('datafim', 'N', 'S', '', 'S' );?>
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

						<a href="javascript:window.location.href='atendefnde.php?modulo=principal/cadastraAtendimento&acao=A'"> <img src="../imagens/gif_inclui.gif" class="link img_middle"  > <b>Incluir Atendimento</b></a>			  		
			  		
			  		</td>
			  		<td style="background-color:#e9e9e9; color:#404040;">
			  		
						<div style="float:center;position:relative;width:240px;">
							<fieldset style="background-color:#ffffff;">
							<legend> <img id="mais_consulta1" style="cursor:pointer;display:none" onclick="exibeConsultaRapida1();" src="../imagens/mais.gif" /> <img  style="cursor:pointer" id="menos_consulta1" onclick="escondeConsultaRapida1();" src="../imagens/menos.gif" /> <b>Consulta por Situação</b></legend>
							<div style="width:230px"></div>
							<table id="consultas_rapidas1" width="100%" >
								<tr>
									<td valign="bottom" height="50%" style="padding-right:15px;">
										<a style="cursor:pointer;<?($ematendimento)? print "font-weight:bold;" : ""; ?>" title="Listar Demandas Em Atendimento." onclick="selecionaEmAtendimento();demandasEmAtendimento();">Em Atendimento<span id="numero_ematendimento"></span></a>
										<input style="display:none" type="radio" id="ematendimento" name="consultaRapida" value="ematendimento" <?=$emtendimento ? 'checked' : ''?>>
									</td>
									<td valign="bottom">
										<a style="cursor:pointer;<?($finalizada)? print "font-weight:bold;" : ""; ?>" onclick="selecionaFinalizada();demandasFinalizada();"  title="Listar Atendimentos Finalizados.">Finalizada<span id="numero_finalizada"></span></a>
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
		$whereFiltro .= " AND a.ateid = {$_REQUEST['numero']} ";

	}
	if($_REQUEST['solicitante']){
		$whereFiltro .= " AND s.soldescricao ILIKE '%{$_REQUEST['solicitante']}%' ";

	}
	if($_REQUEST['estuf']){
		$whereFiltro .= " AND au.estuf = '{$_REQUEST['estuf']}' ";

	}
	if($_REQUEST['muncod']){
		$whereFiltro .= " AND am.muncod = '{$_REQUEST['muncod']}' ";

	}
	if($_REQUEST['dataini'] && $_REQUEST['datafim']){
		$whereFiltro .= " AND a.atedatainclusao >= '".formata_data_sql($_REQUEST['dataini'])."' ";
		$whereFiltro .= " AND a.atedatainclusao <= '".formata_data_sql($_REQUEST['datafim'])."' ";

	}
	
}
	$sql = "SELECT distinct
					'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar Atendimento\" style=\"cursor:pointer;\" onclick=\"window.location=\'atendefnde.php?modulo=principal/listaAtendimento&acao=A&ateid='||a.ateid||'\'\"></center>' as acao,
					'<center>'||a.ateid||'</center>' as ateid,
					'<center>'||to_char(a.atedatainclusao,'dd/mm/YYYY')||'</center>' as datatend,
					a.ateassunto as assunto,
					'' as responsavel,
					'<center>Cadastrado</center>' as situacao,
					'<center>'||u.usunome||'</center>' as cadastradopor
			FROM atendefnde.atendimento a
			INNER JOIN seguranca.usuario u on u.usucpf = a.usucpf
			LEFT JOIN atendefnde.atendimentouf au on au.ateid = a.ateid
			LEFT JOIN atendefnde.atendimentomunicipio am on am.ateid = a.ateid
			LEFT JOIN atendefnde.atendimentosolicitante aso on aso.ateid = a.ateid
			LEFT JOIN atendefnde.solicitante s on s.solid = aso.solid
			WHERE a.atestatus = 'A'
			$whereFiltro
			ORDER BY 2 ASC";
	//dbg($sql,1);
	$cabecalho = array("Ação", "Número", "Data de Atendimento", "Assunto", "Responsável", "Situação", "Cadastrado Por");
	//$tamanho       = array("5%","5%","10%","35%","20%","10%","20%");
	//$alinhamento   = array("center","center","center","left","left","center","center");
	//$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2,"",$tamanho,$alinhamento);
	
	$dados = $db->carregar($sql);
	
	if($dados){
		
		for($i=0;$i<count($dados);$i++){
			$ateid = str_replace("<center>","",str_replace("<center>","",$dados[$i]['ateid']));
			$sql = "select soldescricao from atendefnde.atendimentosolicitante aso
					inner join atendefnde.solicitante s on s.solid = aso.solid
					where aso.ateid = ". (int)$ateid ;
			$solicitante = $db->carregarColuna($sql);
			if($solicitante){
				$dados[$i]['responsavel'] = implode(';<br>',$solicitante);
			}
		}
	}
		
	$db->monta_lista_array($dados,$cabecalho,100,10,'S','center','','');

	
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


		location.href='atendefnde.php?modulo=principal/listaAtendimento&acao=A';
		
	}	

	/*
	function montaSubLista(dmdid)
	{
		td 	   = document.getElementById('td_'+dmdid);
		img_mais   = document.getElementById('img_mais_'+dmdid);
		img_menos   = document.getElementById('img_menos_'+dmdid);
		
		// Faz uma requisição ajax, passando o parametro 'ordid', via POST
		var req = new Ajax.Request('demandas.php?modulo=principal/lista&acao=A', {
				        method:     'post',
				        parameters: '&dmdidAjax=' + dmdid,
				        onComplete: function (res)
				        {	
							td.style.display = '';
							//td.style.padding = '5px';
							img_mais.style.display = 'none';
							img_menos.style.display = '';
							td.innerHTML = res.responseText;
				        }
				  });
	
	}
	

	
	function desmontaSubLista(dmdid)
	{
		td 	   = document.getElementById('td_'+dmdid);
		img_mais   = document.getElementById('img_mais_'+dmdid);
		img_menos   = document.getElementById('img_menos_'+dmdid);
		
		td.innerHTML = '';
		td.style.display = 'none';
		img_mais.style.display = '';
		img_menos.style.display = 'none';
	
	}
	*/
	

function demandasEmAtendimento(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
	document.formulario.submit();
}

function demandasFinalizada(){
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
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
	d.getElementById('numero').value = '';
	d.getElementById('solicitante').value = '';
	d.getElementsByName('estuf').value = '';
	d.getElementsByName('muncod').value = '';
	d.getElementById('dataini').value = '';
	d.getElementById('datafim').value = '';
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