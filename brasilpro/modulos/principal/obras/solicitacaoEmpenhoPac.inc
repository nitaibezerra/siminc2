<?php
include  APPRAIZ."par/classes/Habilita.class.inc";

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

if($_REQUEST['prpid']) {
	
	$_SESSION['brasilpro_var']['prpid'] = $_REQUEST['prpid'];
	
	$sql = "select 
				CASE WHEN estuf IS NULL THEN 'municipal' ELSE 'estadual' END 
			from cte.processobrasilpro  br
			inner join cte.instrumentounidade iu on iu.inuid = br.inuid
			where prpid = ".$_REQUEST['prpid'];
	
	$_SESSION['brasilpro_var']['esfera'] = $db->pegaUm($sql);
}

if($_REQUEST['inuid']) {
	$_SESSION['brasilpro_var']['inuid'] = $_REQUEST['inuid']; 
}

if(!$_SESSION['brasilpro_var']['prpid']) die("<script>
											alert('Processo não encontrado');
											window.close();
										</script>");
$processo = $_REQUEST['processo'];

$largura 	= "250px";
$altura 	= "120px";
$id 		= "div_auth";
?>
<html>
	<head>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="js/brasilproobras.js"></script>
		<style>
			#listaSub100Empenhada thead tr td, #listaSubASeremEmpenhada thead tr td
			{
				background-color: #F7F7F7;
				font-weight: bold;
				font-size: 12px;
				font-family: arial;
			}
			
			.popup_alerta
			{
				width:<?php echo $largura ?>;
				height:<?php echo $altura ?>;
				position:absolute;
				z-index:0;
				top:50%;
				left:50%;
				margin-top:-<?php echo $altura/2 ?>;
				margin-left:-<?php echo $largura/2 ?>;
				border:solid 2px black;
				background-color:#FFFFFF;
				display:none
			}
			.popup_alerta2
			{
				width:90%;
				height:90%;
				position:absolute;
				z-index:0;
				top:50%;
				left:30%;
				margin-top:-250;
				margin-left:-250;
				border:solid 2px black;
				background-color:#FFFFFF;
				display:none
			}
		</style>
		<script type="text/javascript">
			$(document).ready(function() {
				carregarListaSubacao();
				carregarListaEmpenhoProcesso('','<? echo $processo ?>');
			});
			
			function carregarListaSubacao() {
				$.ajax({
			   		type: "POST",
			   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
			   		data: "requisicao=listaSubacao",
			   		async: false,
			   		success: function(msg){
			   			document.getElementById('divListaSubacoes').innerHTML = msg;
			   			if(msg.search("checkbox") < 0 ){
			   				//document.getElementById('formulario').innerHTML = "";
			   			}
			   		}
				});
				
			}
			
			function carregarListaEmpenhoProcesso(empid,processo) {

				$.ajax({
			   		type: "POST",
			   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
			   		data: "requisicao=listaEmpenhoProcesso&empid=" + empid + "&empnumeroprocesso=" + processo,
			   		async: false,
			   		success: function(msg){
			   			document.getElementById('listaempenhoprocesso').innerHTML = msg;
			   		}
				});
				
			}
			
			function visEmp(){
				var dados = $('#formEmpenharSubacoes').serialize();
				
				$.ajax({
					type: "POST",
					url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
					data: "requisicao=solicitarEmpenho&tipo=visualiza&"+dados,
					async: false,
					success: function(msg){
						document.getElementById('visXML').style.display='block';
						document.getElementById('visXMLDadosEmpenho').innerHTML=msg;
				}
				});
			}
		
			function marcarChk(obj) {
				var linha = obj.parentNode.parentNode;
			
				if(obj.checked) {
					document.getElementById('id_'+obj.value).className="normal";
					document.getElementById('id_'+obj.value).readOnly=false;
				} else {
					document.getElementById('id_'+obj.value).className="disabled";
					document.getElementById('id_'+obj.value).readOnly=true;
				}
				
				var tabela = obj.parentNode.parentNode.parentNode;
				calcularTotal(tabela);
			}
			
			function calcularTotal(tbl) {
				
				var total=0;
				for(var i=0;i<tbl.rows.length;i++) {
					var input = tbl.rows[i].cells[0].childNodes[0];
					if(input) {
						if(input.checked) {
							total = total + parseFloat(replaceAll(replaceAll(replaceAll(tbl.rows[i].cells[7].innerHTML, '<br>', ''), '.', ''), ',', '.'));
						}
					}
				}
				document.getElementById('id_total').value= mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
			}
			
			function calculaEmpenho(obj) {
				var linha = obj.parentNode.parentNode;
				var valor = parseFloat(replaceAll(replaceAll(replaceAll(linha.cells[3].innerHTML, '<br>', ''), '.', ''), ',', '.'));
				//var valorEmpenhado = linha.cells[4].childNodes[2].value;
				var total = valor*obj.value/100;
				var total_mac = mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
				linha.cells[7].innerHTML = total_mac;
				linha.cells[6].childNodes[1].value = total_mac;
				//linha.cells[5].childNodes[1].value = total;
				var tabela = obj.parentNode.parentNode.parentNode;
				calcularTotal(tabela);
			
			}
			
			function verificaPreenchimentoPorcentagem(obj){
			//alert(obj);
			// 0 - &nbsp; 1- Nº do Empenho 2- Subação 3 - Valor da Subação 4 - % Empenhado 5 - Valor Empenhado 6 - % a Empenhar 7 - Valor a Empenhar
				var linha 			= obj.parentNode.parentNode;
				var valorSubacao 	= linha.cells[3].childNodes[0].value;
				var valorInformado 	= linha.cells[7].childNodes[0].value;
			
				if( valorInformado > valorSubacao ){
					alert('O valor informado para empenho ultrapassa 100% do valor da Subação.');
					linha.cells[7].childNodes[0].value = 0;
				}
				
			}
			
			function solicitarEmpenhoPar() {
				
				var form = document.getElementById('formEmpenharSubacoes');
				
				var usuario = document.getElementById('wsusuario').value;
				var senha = document.getElementById('wssenha').value;
				
				var marcado=false;
				for(var i=0;i<form.elements.length;i++) {
					if(form.elements[i].type) {
						if(form.elements[i].type == "checkbox" && form.elements[i].checked == true) {
							marcado = true;
						}
					}
				}
				if(!marcado) {
					alert('Nenhuma Subação selecionada');
					return false;
				}
				
				if(!usuario) {
					alert('Favor informar o usuário!');
					return false;
				}
				
				if(!senha) {
					alert('Favor informar a senha!');
					return false;
				}
				
			
				divCarregando();
				
				var dadosSubacao = $('#formEmpenharSubacoes').serialize();
				//var filtrosobras = $('#formEmpenharSubacoes').serialize();
			
				$.ajax({
			   		type: "POST",
			   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
			   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarEmpenho&"+dadosSubacao,
			   		async: false,
			   		success: function(msg){
			   			alert(msg);
			   		}
				});
				
				carregarListaSubacao();
				carregarListaEmpenhoProcesso();
			
				divCarregado();
				
			}
			
			function cancelarEmpenhoWS() {

				var wsusuario = document.getElementById('ws_usuario_cancela').value;
				var wssenha = document.getElementById('ws_senha_cancela').value;
				var wsempid = document.getElementById('ws_empid').value;
				var wsprocesso = document.getElementById('ws_processo').value;
				
				if(!wsusuario){
					alert('Favor informar o nome de usuário!');
					return false;
				}
				if(!wssenha){
					alert('Favor informar a senha!');
					return false;
				}
				
				document.getElementById('div_auth_cancela').style.display = 'none';
				
				divCarregando();
				
				$.ajax({
			   		type: "POST",
			   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
			   		data: "requisicao=cancelarEmpenho&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
			   		async: false,
			   		success: function(msg){alert(msg);}
				});
				
				carregarListaEmpenhoProcesso(wsempid,wsprocesso);
				
				divCarregado();
				
			}
			
			function cancelarEmpenho(empid,processo) {
			
				document.getElementById('ws_usuario_consulta').value;
				document.getElementById('ws_senha_consulta').value;
				document.getElementById('ws_empid').value = empid;
				document.getElementById('ws_processo').value = processo;
				
				document.getElementById('div_auth_cancela').style.display = 'block';
				document.body.scrollTop = 0;
			}
			
			
			function consultarEmpenho(empid,processo) {
			
				document.getElementById('ws_usuario_consulta').value;
				document.getElementById('ws_senha_consulta').value;
				document.getElementById('ws_empid').value = empid;
				document.getElementById('ws_processo').value = processo;
				
				document.getElementById('div_auth_consulta').style.display = 'block';
				document.body.scrollTop = 0;
			}
			
			
			function consultarEmpenhoWS() {
			
				var wsusuario = document.getElementById('ws_usuario_consulta').value;
				var wssenha = document.getElementById('ws_senha_consulta').value;
				var wsempid = document.getElementById('ws_empid').value;
				var wsprocesso = document.getElementById('ws_processo').value;
				
				if(!wsusuario){
					alert('Favor informar o nome de usuário!');
					return false;
				}
				if(!wssenha){
					alert('Favor informar a senha!');
					return false;
				}
				
				document.getElementById('div_auth_consulta').style.display = 'none';
				
				divCarregando();
				
				$.ajax({
			   		type: "POST",
			   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
			   		data: "requisicao=consultarEmpenho&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
			   		async: false,
			   		success: function(msg){alert(msg);}
				});
				
				carregarListaEmpenhoProcesso(wsempid,wsprocesso);
				
				divCarregado();
				
			}
			
		</script>
	</head>
	<body>
		<?php
			monta_titulo( $titulo_modulo, '&nbsp;' );
			cabecalhoSolicitacaoEmpenhoPar();
		?>
		<form id="formEmpenharSubacoes">
		<div id="divListaSubacoes">Carregando...</div>
		<table id="total" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td>
					<b>Valor total de empenho:</b>
					<input type="text" id="id_total" name="name_total" size="15" class="normal" style="text-align:right;" readonly="readonly" value="0,00">
					<input type="button" name="solicitar" id="solicitar" value="Solicitar empenho" onclick="document.getElementById('div_auth').style.display='block'" />
					<?php
					if( in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) ){ ?>
						<input type=button id=visualizar name=visualizar  value=Visualizar XML onclick=visEmp(); />
					<?php } ?>
				</td>
			</tr>
		</table>
		</form>
		<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td id="listaempenhoprocesso">Carregando...</td>
			</tr>
		</table>
		
		<!-- Div para solicitação de Empenho -->
		<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
			<div style="width:100%;text-align:right">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
			</div>
			<div style="padding:5px;text-align:justify;">
				<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td width="30%" class="SubtituloDireita">Usuário:</td>
					<td><?php echo campo_texto("wsusuario","S","S","Usuário","18","","","","","","","id='wsusuario'") ?></td>
				</tr>
				<tr>
					<td class="SubtituloDireita">Senha:</td>
					<td>
						<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="19" id="wssenha" name="wssenha">
						<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					</td>
				</tr>
				<tr>
					<td align="center" bgcolor="#D5D5D5" colspan="2">
						<input type="button" name="btn_enviar" onclick="solicitarEmpenhoPar()" value="ok" />
						<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
					</td>
				</tr>
				</table>
			</div>
		</div>
		<div id="visXML" class="popup_alerta2 <?php echo $classeCSS ?>" >
			<div style="width:100%;text-align:right">
				<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('visXML').style.display='none'" />
			</div>
			<div style="padding:5px;text-align:justify;">
				<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td width="10%" class="SubtituloDireita">Dados do Empenho:</td>
					<td id="visXMLDadosEmpenho"></td>
				</tr>
				</table>
			</div>
		</div>
		
<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_consulta";
?>
<style>
	.popup_alerta_consulta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_consulta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" class="SubtituloDireita">Usuário:</td>
				<td><?php echo campo_texto("ws_usuario_consulta","S","S","Usuário","18","","","","","","","id='ws_usuario_consulta'") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="19" id="ws_senha_consulta" name="ws_senha_consulta">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td align="center" bgcolor="#D5D5D5" colspan="2">
					<input type="hidden" name="ws_empid" id="ws_empid" value="" />
					<input type="hidden" name="ws_processo" id="ws_processo" value="" />
					<input type="button" name="btn_enviar" onclick="consultarEmpenhoWS()" value="ok" />
					<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
				</td>
			</tr>
		</table>
	</div>
</div>

<?php
$largura = "250px";
$altura = "120px";
$id = "div_auth_cancela";
?>
<style>
	.popup_alerta_cancela{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div id="<?php echo $id ?>" class="popup_alerta_cancela <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" class="SubtituloDireita">Usuário:</td>
				<td><?php echo campo_texto("ws_usuario_cancela","S","S","Usuário","18","","","","","","","id='ws_usuario_cancela'") ?></td>
			</tr>
			<tr>
				<td class="SubtituloDireita">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="" size="19" id="ws_senha_cancela" name="ws_senha_cancela">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td align="center" bgcolor="#D5D5D5" colspan="2">
					<input type="hidden" name="ws_empid" id="ws_empid" value="" />
					<input type="hidden" name="ws_processo" id="ws_processo" value="" />
					<input type="button" name="btn_enviar" onclick="cancelarEmpenhoWS()" value="ok" />
					<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
				</td>
			</tr>
		</table>
	</div>
</div>
		
		
		
	</body>
</html>
<?php 
function executarEmpenho($dados) {
	global $db;

	$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM cte.processobrasilpro WHERE prpid='".$_SESSION['brasilpro_var']['prpid']."'");		
//	$res_se = solicitarEmpenho($dados, null ,null );
//	return true;
	if($dadosse['prptipoexecucao'] == 'C'){
		$obHabilita = new Habilita();
		$cnpj 		= $obHabilita->pegaCnpj($_SESSION['brasilpro_var']['inuid']);
		$habilitado = $obHabilita->consultaHabilitaEntidade($cnpj);
		
		if($habilitado == 'Habilitado'){
			$res_sp = solicitarProcesso($dados);
			$res_cc = consultarContaCorrente($dados);
			if($res_cc) $res_sc = solicitarContaCorrente($dados);
					$dadosConvenioFNDE = solicitarConvenio($dados);
					$nu_convenio  = (int) $dadosConvenioFNDE->nu_convenio;
					$an_exercicio = (int) $dadosConvenioFNDE->an_exercicio;
					if($dadosConvenioFNDE){
						$res_se = solicitarEmpenho($dados, $nu_convenio ,$an_exercicio );
					}
		}else{
			echo $habilitado;
		}
		
	}else{
		$res_sp = solicitarProcesso($dados);
		$res_cc = consultarContaCorrente($dados);
		if($res_cc){
			$res_sc = solicitarContaCorrente($dados);
		}
		$res_se = solicitarEmpenho($dados, null ,null );
	}
}


function solicitarEmpenho($dados, $nu_convenio = null ,$an_exercicio = null) 
{
	
	global $db;
	
	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		$senha   = $dados['wssenha'];
		//$usuario = 'MECTIAGOT';
		//$senha   = 'M3135689';

	    $dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM cte.processobrasilpro WHERE prpid='".$_SESSION['brasilpro_var']['prpid']."'");
		
	    $PI = $db->pegaLinha("select sptplanointerno 
								from cte.subacaoparecertecnico s 
								inner join cte.subacaoindicador sb on sb.sbaid = s.sbaid and s.sptano = date_part('year', now())
								inner join cte.processobrasilpro p on p.prpid = s.prpid where p.prpid='".$_SESSION['brasilpro_var']['prpid']."'");
	
	    if($dadosse) {
				$co_fonte_recurso_solic="0112000000";
	        	$nu_processo=$dadosse['prpnumeroprocesso'];
				//$co_natureza_despesa_solic="33304100";
				$co_natureza_despesa_solic="33900000";
				$co_plano_interno_solic= $PI['sptplanointerno'];
				$co_ptres_solic="043936";
				$frpfuncionalprogramatica="12363203182520001";

				$sql = "SELECT frpcodigo, frpsaldo, frpfuncionalprogramatica, frptitulo FROM cte.fonterecursobrasilpro WHERE frpfuncionalprogramatica='".$frpfuncionalprogramatica."' ORDER BY frpid";
				
				$fonterecursobrasilpro = $db->carregar($sql);

				if($fonterecursobrasilpro[0]) {
					foreach($fonterecursobrasilpro as $frpsaldo) {
						$sql = "SELECT sum(e.empvalorempenho) as vlr FROM cte.empenho e 
								INNER JOIN cte.processobrasilpro p ON p.prpnumeroprocesso = e.empnumeroprocesso 
								WHERE e.empfonterecurso='".$frpsaldo['frpcodigo']."'";
						$valor_recurso_pac = $db->pegaUm($sql);
						$total_parcial = ($valor_recurso_pac + str_replace(array(".",","),array("","."),$dados['name_total']));
						if($total_parcial <= $frpsaldo['frpsaldo']) {
							$co_fonte_recurso_solic=$frpsaldo['frpcodigo'];
							break;
						}
					}
					if(!$co_fonte_recurso_solic) {
					    echo "------ SOLICITAÇÃO DE EMPENHO ------\n\n";
					    echo "MSG SIMEC : Fonte de recurso ".$frpsaldo['frpcodigo']." - ".$frpsaldo['frptitulo']." finalizada \n";
					    echo "Saldo da fonte : ".$frpsaldo['frpsaldo']." < (Total de empenho : ".$valor_recurso_pac." + Valor empenho atual : ".str_replace(array(".",","),array("","."),$dados['name_total']).")";
					    return false;
					}
					
				}


        	//}
        }
		
		if($_SESSION['brasilpro_var']['esfera']=='estadual') {
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("SELECT 
												trim(prpcnpj)
					 				   		FROM 
					 				   			cte.processobrasilpro 
					 				   		WHERE 
					 				   			prpnumeroprocesso = '{$dadosse['prpnumeroprocesso']}'");
        }else{
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("SELECT 
												ent.entnumcpfcnpj
					 				   		FROM 
					 				   			entidade.entidade ent
						 					INNER JOIN 
						 						entidade.funcaoentidade fen ON fen.entid = ent.entid
						 					INNER JOIN 
						 						entidade.endereco ende ON ende.entid = ent.entid
						 					WHERE 
						 						fen.funid=1 
						 					AND 
						 						ende.muncod='".$dadosse['muncod']."'");
        }
        
		if($dadosse['prptipoexecucao'] == 'C'){
	   		$co_centro_gestao_solic="61500000000";
	   		$co_programa_fnde="03";
			$an_convenio=$an_exercicio;
			$nu_convenio=$nu_convenio;
			$co_tipo_empenho="5";
			$nu_sistema="2";
	    }else if($dadosse['prptipoexecucao'] == 'T'){
	        $co_centro_gestao_solic="61400000000";
	        $co_programa_fnde="CM"; //ED
	        $an_convenio=null;
			$nu_convenio=null;
			$co_tipo_empenho="3";
			$nu_sistema="7";
	    }
		
		// nulo
		$nu_empenho_original=null;
		// nulo
		$an_exercicio_original=null;
		// total do empenho, calculado na tela
		$vl_empenho=str_replace(array(".",","),array("","."),$dados['name_total']);
		// constante=01
		$co_especie_empenho="01";
		// constante=1
		$co_esfera_orcamentaria_solic="1";
		// constante=2
		$co_observacao="2";

		$co_descricao_empenho="0010";
		// constante=15253
		$co_gestao_emitente="15253";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
		// constante=153173
		$co_unidade_gestora_emitente="153173";
		// constante=26298
		$co_unidade_orcamentaria_solic="26298";
		// nulo
		$nu_proposta_siconv=null;

    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
		<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
		<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
		<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
		<vl_empenho>$vl_empenho</vl_empenho>
		<nu_processo>$nu_processo</nu_processo>
		<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
		<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
		<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
		<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
		<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
		<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
		<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
		<an_convenio>$an_convenio</an_convenio>
		<nu_convenio>$nu_convenio</nu_convenio>
		<co_observacao>$co_observacao</co_observacao>
		<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
		<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
		<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
		<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
		<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
		<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
		<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

	if( $dados['tipo'] == 'visualiza' ){
		echo '<pre>';		
		echo simec_htmlentities($arqXml);
		exit;
    }

	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
	} else {
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
	}

	$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
			->execute();
		
  /*
		   	$xml =  <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<response><header><app>SIGEF</app><version>v0.0.3</version><created>2011-05-20T17:42:11</created></header><status><result>1</result><message><code>1</code><text>Solicitação realizada com sucesso!</text></message></status><body><nu_seq_ne>170245</nu_seq_ne></body></response>

XML;
	*/
	
				
		$xmlRetorno = $xml;
//echo $xmlRetorno;
	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ SOLICITAÇÃO DE EMPENHO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
		
		
		$result = (integer) $xml->status->result;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}

			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'solicitarEmpenho - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    return false;
		} else {

			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'solicitarEmpenho - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);

			$sql = "INSERT INTO cte.empenho(
            empcnpj,
            empnumerooriginal,
            empanooriginal,
            empvalorempenho,
            empnumeroprocesso,
            empcodigoespecie,
            empcodigopi,
            empcodigoesfera,
            empcodigoptres,
            empfonterecurso,
            empcodigonatdespesa,
            empcentrogestaosolic,
            empanoconvenio,
            empnumeroconvenio,
            empcodigoobs,
            empcodigotipo,
            empdescricao,
            empgestaoeminente,
            empunidgestoraeminente,
            empprogramafnde,
            empnumerosistema,
            usucpf,
            empprotocolo,
            empsituacao
            )
		    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
		    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
		    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
		    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
		            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
		            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
		            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
		            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
		            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
		            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
		            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
		            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
		            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
		            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
		            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
		            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
		            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
		            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
		            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
		            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
		            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
		            '".$_SESSION['usucpf']."',
		            '".$xml->body->nu_seq_ne."',
		            '8 - SOLICITAÇÃO APROVADA'
		            ) RETURNING empid;";

			$empid = $db->pegaUm($sql);

			$sql = "INSERT INTO cte.historicoempenho(
            		usucpf, empid, hepdata, empsituacao)
    				VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '8 - SOLICITAÇÃO APROVADA');";

			$db->executar($sql);
		
			if($dados['chk']) {
				foreach($dados['chk'] as $sbaid)
				{
					$valorPorSubacao = str_replace(array(".",","),array("","."),number_format($dados['vlrTotal_'.$sbaid], 2, ',', '.'));
//					$valorPorSubacao = str_replace(array(".",","),array("","."),$dados['vlr_'.$sbaid]);
					$sql = "INSERT INTO cte.empenhosubacao(
            				sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
    						VALUES ('".$sbaid."', '".$empid."', '".$dados['name_'.$sbaid]."', '".$valorPorSubacao."', '{$dados['anosubacao_'.$sbaid]}');"; // mudar o ano depois

					$db->executar($sql);
				}
			}

			$db->commit();

			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";


	}
}

function cancelarEmpenho($dados) {
	global $db;

	try {
		$data_created = date("c");
		
		$sql = "SELECT count(*) as num FROM cte.historicoempenho WHERE empid='".$dados['empid']."' AND empsituacao='2 - EFETIVADO'";
		$historicoefetivado = $db->pegaUm($sql);

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$nu_seq_ne = "73289";
		} else {
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];

		    $dadosemp = $db->pegaLinha("SELECT * FROM cte.empenho WHERE empid='".$dados['empid']."'");

	        if($dadosemp) {
	        	$nu_seq_ne = $dadosemp['empprotocolo'];
	        }

		}
		
		if($historicoefetivado>0) {
			
	      	$nu_processo=$dadosemp['empnumeroprocesso'];
			// constante=4042
			//$co_natureza_despesa_solic="33304100";
			$co_natureza_despesa_solic="33900000";
			
			if($dadosemp['empprogramafnde'] == 'BW') {
				// constante=MEC00001
				$co_plano_interno_solic="MEC00001";
				// constante=037825
				$co_ptres_solic="043936";
				// constante=12365144812KU0001
				$frpfuncionalprogramatica="12363203182520001";
        	} else {
				// constante=MEC00002
				$co_plano_interno_solic="MEC00002";
				// constante=037826
				$co_ptres_solic="043936";
				// constante=12365144812KV0001
				$frpfuncionalprogramatica="12363203182520001";
        	}
			
			$nu_cnpj_favorecido=$dadosemp['empcnpj'];
			
			if($dadosemp['empnumero']) {
				$arrNumero = explode("NE",$dadosemp['empnumero']);
				// nulo
				$nu_empenho_original=$arrNumero[1];
				// nulo
				$an_exercicio_original=$arrNumero[0];
			} else {
				// nulo
				$nu_empenho_original=null;
				// nulo
				$an_exercicio_original=null;
			}
			
			// total do empenho, calculado na tela
			$vl_empenho=str_replace(array(".",","),array("","."),$dadosemp['empvalorempenho']);
			// constante=01
			$co_especie_empenho="03";
			// constante=1
			$co_esfera_orcamentaria_solic="1";
			// constante=69500000000
			$co_centro_gestao_solic="69500000000";
			// nulo
			$an_convenio=null;
			// nulo
			$nu_convenio=null;
			// constante=2
			$co_observacao="2";
			// constante=3
			$co_tipo_empenho="3";
			// constante=0010
			$co_descricao_empenho="0010";
			// constante=15253
			$co_gestao_emitente="15253";
	        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
	        $co_programa_fnde=$dadosemp['empprogramafnde'];
			// constante=153173
			$co_unidade_gestora_emitente="153173";
			// constante=26298
			$co_unidade_orcamentaria_solic="26298";
			// nulo
			$nu_proposta_siconv=null;
			
			
        	$dadoscc = $db->pegaLinha("SELECT prpnumeroprocesso,prptipoexecucao, prpbanco, prpagencia, muncod, prptipo, inuid FROM cte.processobrasilpro WHERE prpid='".$_SESSION['brasilpro_var']['prpid']."'");
		
			
			if($dadoscc['prptipoexecucao'] == 'C'){
	   			$nu_sistema="2";
	    	}else if($dadoscc['prptipoexecucao'] == 'T'){ 
		        $nu_sistema="7";
	    	}
       
	
	
	    	$arqXml = <<<XML
	<?xml version='1.0' encoding='iso-8859-1'?>
	<request>
		<header>
			<app>string</app>
			<version>string</version>
			<created>$data_created</created>
		</header>
		<body>
			<auth>
				<usuario>$usuario</usuario>
				<senha>$senha</senha>
			</auth>
			<params>
			<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
			<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
			<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
			<vl_empenho>$vl_empenho</vl_empenho>
			<nu_processo>$nu_processo</nu_processo>
			<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
			<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
			<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
			<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
			<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
			<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
			<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
			<an_convenio>$an_convenio</an_convenio>
			<nu_convenio>$nu_convenio</nu_convenio>
			<co_observacao>$co_observacao</co_observacao>
			<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
			<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
			<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
			<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
			<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
			<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
			<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
			<nu_sistema>$nu_sistema</nu_sistema>
			</params>
		</body>
	</request>
XML;
	
			if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
				$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
			} else {
				$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
			}
	
			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
					->execute();
	
			$xmlRetorno = $xml;
	
		    $xml = simplexml_load_string( stripslashes($xml));
	
		    echo "------ SOLICITAÇÃO DE EMPENHO (CANCELAMENTO) ------\n\n";
			echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
			
			$result = (integer) $xml->status->result;
	
			if($result) $hwpwebservice = "cancelarEmpenho2 - Sucesso";
			else $hwpwebservice = "cancelarEmpenho2 - Erro";
			
			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'".$hwpwebservice."',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
			
			$db->executar($sql);
			$db->commit();

		}


    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'cancelar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ CANCELAMENTO DE EMPENHO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		$result = (integer) $xml->status->result;

		if($result) {

			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'cancelarEmpenho - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			$db->executar("UPDATE cte.empenho SET empsituacao='CANCELADO'
					   	  WHERE empid='".$dados['empid']."'");

			$db->executar("INSERT INTO cte.historicoempenho(
            			   usucpf, empid, hepdata, empsituacao)
    					   VALUES ('".$_SESSION['usucpf']."', '".$dados['empid']."', NOW(), 'CANCELADO');");

			$db->executar("DELETE FROM cte.empenhosubacao WHERE empid='".$dados['empid']."'");

			$db->commit();

		} else {

			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;

			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}

			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'cancelarEmpenho - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		   	return false;
		}


	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Cancelar Empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Cancelar Empenho no SIGEF: $erroMSG";


	}
}




function solicitarProcesso($dados) {
	global $db;
	
    $dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prptipoexecucao, muncod, prpbanco, prpagencia, prpdatainclusao, usucpf, prpseqconta, prptipo, seq_conta_corrente, nu_conta_corrente, inuid
    						   FROM cte.processobrasilpro
    						   WHERE prpid='".$_SESSION['brasilpro_var']['prpid']."'");
	
    if($dadosse) {
    	$an_processo = date("Y");
    	$nu_processo=$dadosse['prpnumeroprocesso'];
    	$tp_processo=1; // O que vai ser no PAR

    	if($dadosse['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde="03";
	    }else if($dadosse['prptipoexecucao'] == 'T'){ 
	        $co_programa_fnde="CM"; //ED
	    }
    	
    	
		if($_SESSION['brasilpro_var']['esfera']=='estadual') {
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("SELECT 
												trim(prpcnpj)
					 				   		FROM 
					 				   			cte.processobrasilpro 
					 				   		WHERE 
					 				   			prpnumeroprocesso = '{$dadosse['prpnumeroprocesso']}'");
        }else{
        	// CNPJ da prefeitura
			$nu_cnpj_favorecido=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadosse['muncod']."'");
		}
    }
  
	$data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	//$usuario = 'MECTIAGOT';
	//$senha   = 'M3135689';

    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<params>
      	<nu_cnpj>$nu_cnpj_favorecido</nu_cnpj>
      	<nu_processo>$nu_processo</nu_processo>
      	<tp_processo>$tp_processo</tp_processo>
      	<an_processo>$an_processo</an_processo>
      	<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;
//echo $arqXml;
//return true;

	//	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
	//		$urlWS = 'http://172.20.200.116/webservices/corp/integracao/web/dev.php/processo/solicitar';
	//	} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/corp/index.php/processo/solicitar';
	//	}
		
		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
				->execute();

		$xmlRetorno = $xml;
		
	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ SOLICITAÇÃO DE PROCESSO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		$result = (integer) $xml->status->result;
		
		if(!$result) {
		
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
					echo "\n";
				}
			}

			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'solicitarProcesso - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
	
			$db->executar($sql);
			$db->commit();

		    return false;
		} else {

			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'solicitarProcesso - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}

}


function consultarContaCorrente($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

        $proseqconta = $db->pegaUm("SELECT prpseqconta FROM cte.processobrasilpro WHERE prpid='".$_SESSION['brasilpro_var']['prpid']."'");
		
    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <seq_solic_cr>$proseqconta</seq_solic_cr>
		</params>
	</body>
</request>
XML;

	//	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
	//		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
	//	} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	//	}

		if($proseqconta) {

			$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
					->execute();

			$xmlRetorno = $xml;

		    $xml = simplexml_load_string( stripslashes($xml));

		    if($xml->body->row->seq_conta) {
		    	$db->executar("UPDATE cte.processobrasilpro SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE prpseqconta='".$proseqconta."'");
		    	$db->commit();
		    }

			echo "------ CONSULTA DE CONTA CORRENTE ------\n\n";
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Detalhes da consulta ***\n\n";
			echo "* Data movimento:".(($xml->body->row->dt_movimento)?$xml->body->row->dt_movimento:'-')."\n";
			echo "* Fase solicitação:".(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-')."\n";
			echo "* Entidade:".(($xml->body->row->ds_razao_social)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social):'-')."(".(($xml->body->row->nu_identificador)?$xml->body->row->nu_identificador:'-').")\n\n";

			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'consultarContaCorrente',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    $result = (integer) $xml->status->result;

		    if($result) {
		    	return false;
		    } else {
		    	return true;
		    }

		} else {
			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";

	}
}

// colocar tipo de subação e colocar um filtro tb.
function cabecalhoSolicitacaoEmpenhoPar()  {
	
	global $db;
	
	/*
	$sql = "select 
				CASE WHEN estuf IS NULL THEN 'municipal' ELSE 'estadual' END 
			from cte.processobrasilpro  br
			inner join cte.instrumentounidade iu on iu.inuid = br.inuid
			where prpid = ".$_REQUEST['prpid'];
	 */
	
	$arrDados = $db->pegaLinha("SELECT m.muncod,
									   m.estuf,
									   m.mundescricao,
									   iu.estuf as estado,
									   p.prpnumeroprocesso
									--   CASE WHEN p.prptipo='P' THEN 'Padrão' ELSE 'Ver' END as tipoobra,
									--   p.prptipo
								FROM 
									cte.processobrasilpro p
								INNER JOIN 
									cte.instrumentounidade iu on iu.inuid = p.inuid
								LEFT JOIN 
									territorios.municipio m ON m.muncod = p.muncod
								WHERE 
									p.prpid='{$_SESSION['brasilpro_var']['prpid']}'");
	
	echo "<table border=0 cellpadding=3 cellspacing=0 class=listagem width=95% align=center>";
	
	if($_SESSION['brasilpro_var']['esfera']!='estadual'){
		echo "<tr>";
		echo "<td class=SubTituloDireita>UF:</td>";
		echo "<td>".$arrDados['estuf']."</td>";
		echo "</tr>";
		echo "<tr>";
		echo "<td class=SubTituloDireita>Município:</td>";
		echo "<td>".$arrDados['mundescricao']."</td>";
		echo "</tr>";
	}else{
		echo "<tr>";
		echo "<td class=SubTituloDireita>UF:</td>";
		echo "<td>".$arrDados['estado']."</td>";
		echo "</tr>";	
	}
	echo "<tr>";
	echo "<td width=\"180\" class=SubTituloDireita>Nº processo:</td>";
	echo "<td>".$arrDados['prpnumeroprocesso']."</td>";
	echo "</tr>";
	echo "</table>";
}


function solicitarContaCorrente($dados) {
	global $db;
	
	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

        $dadoscc = $db->pegaLinha("SELECT prpnumeroprocesso,prptipoexecucao, prpbanco, prpagencia, muncod, prptipo, inuid FROM cte.processobrasilpro WHERE prpid='".$_SESSION['brasilpro_var']['prpid']."'");
        		
        if($dadoscc) {
	        // numero do processo (No desenvolvimento é fixo)
        //	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
        //	   $_SESSION['baselogin'] == "simec_espelho_producao" ){
        	   	//$nu_processo='23034655466200900';
        //	   	$nu_processo=$dadoscc['prpnumeroprocesso'];//234000005642011
        //	} else {
	        	$nu_processo=$dadoscc['prpnumeroprocesso'];
        //	}

	        // constante=001
	        $nu_banco=$dadoscc['prpbanco'];
	        // esperando envio
	        $nu_agencia=$dadoscc['prpagencia'];
        }

	if($_SESSION['brasilpro_var']['esfera']=='estadual') {
		
        	// CNPJ da prefeitura
			$nu_identificador=$db->pegaUm("SELECT 
												trim(prpcnpj)
					 				   		FROM 
					 				   			cte.processobrasilpro 
					 				   		WHERE 
					 				   			prpnumeroprocesso = '{$dadoscc['prpnumeroprocesso']}'");
			
        }else{
        	
			// CNPJ da prefeitura
			$nu_identificador=$db->pegaUm("SELECT 
												ent.entnumcpfcnpj
						 				   FROM 
												entidade.entidade ent
						 				   INNER JOIN 
						 				   		entidade.funcaoentidade fen ON fen.entid = ent.entid
						 				   INNER JOIN 
						 				   		entidade.endereco ende ON ende.entid = ent.entid
						 				   WHERE 
						 				   		fen.funid=1 
						 				   	AND 
						 				   		ende.muncod='".$dadoscc['muncod']."'");			
        }
			
  		// constante=1
        $tp_identificador="1";

        // constante=nulo
        $nu_conta_corrente=null;
        // constante=01
        $tp_solicitacao="01";
        // constante=0032
        $motivo_solicitacao="0032";
        // constante=nulo
        $convenio_bb=null;
        // constante=N
        $tp_conta="N";
        // condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
        
		if($dadoscc['prptipoexecucao'] == 'C'){
	   		$co_programa_fnde="03";
	   		$nu_sistema="2";
	    }else if($dadoscc['prptipoexecucao'] == 'T'){ 
	        $co_programa_fnde="CM"; //ED
	        $nu_sistema="7";
	    }
       



    $arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_identificador>$nu_identificador</nu_identificador>
        <tp_identificador>$tp_identificador</tp_identificador>
        <nu_processo>$nu_processo</nu_processo>
        <nu_banco>$nu_banco</nu_banco>
        <nu_agencia>$nu_agencia</nu_agencia>
        <nu_conta_corrente>$nu_conta_corrente</nu_conta_corrente>
        <tp_solicitacao>$tp_solicitacao</tp_solicitacao>
        <motivo_solicitacao>$motivo_solicitacao</motivo_solicitacao>
        <convenio_bb>$convenio_bb</convenio_bb>
        <tp_conta>$tp_conta</tp_conta>
        <nu_sistema>$nu_sistema</nu_sistema>
        <co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;

	//	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
	//	   $_SESSION['baselogin'] == "simec_espelho_producao" ){
	//		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
	//	} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	//	}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

	    echo "------ SOLICITAÇÃO DE CONTA CORRENTE ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		$result = (integer) $xml->status->result;
		if(!$result) {
			echo "*** Descrição do erro ***\n\n";
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
				}
			}

			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'solicitarContaCorrente - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

		    return false;
		} else {
			
		    $db->executar("UPDATE cte.processobrasilpro SET prpseqconta='".$xml->body->seq_solic_cr."', seq_conta_corrente='".$xml->body->nu_seq_conta."' WHERE prpid='".$_SESSION['brasilpro_var']['prpid']."'");

			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'solicitarContaCorrente - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";

	}
}



function listaSubacao($dados) {
	global $db;
	
	// Lista de Subações 100% empenhadas
	$sql = "SELECT  '' as botao,
				                        e.empnumero as numeroempenho,
				                        s.sbadsc as descricaosubacao,
				                        es.eobpercentualemp as percentualempenhado,
				                        es.eobvalorempenho as valorempenhado
								FROM cte.instrumentounidade   iu
								INNER JOIN cte.pontuacao        		po on po.inuid   = iu.inuid
								INNER JOIN cte.acaoindicador   			a  on a.ptoid   = po.ptoid
								INNER JOIN cte.subacaoindicador     	s  on s.aciid   = a.aciid
								INNER JOIN cte.processobrasilpro    	p  ON p.inuid = iu.inuid
								INNER JOIN cte.subacaoparecertecnico    sd ON sd.sbaid = s.sbaid AND p.prpid = sd.prpid
								INNER JOIN  cte.empenhosubacao   		es ON es.sbaid = s.sbaid AND es.eobano = sd.sptano
								INNER JOIN cte.empenho                  e  ON e.empid = es.empid  
								WHERE
								
								 true = (      SELECT CASE WHEN pr.prptipoexecucao = 'C'
			                                               THEN ems.eobpercentualemp = 99
			                                               ELSE ems.eobpercentualemp = 100
			                                               END as d
			                                   FROM cte.processobrasilpro pr
			                                   INNER JOIN cte.empenho e on e.empnumeroprocesso = pr.prpnumeroprocesso
			                                   INNER JOIN cte.empenhosubacao ems on ems.empid = e.empid
			                                   WHERE  pr.prpid =  782
			                                   )								
								AND  p.prpid = {$_SESSION['brasilpro_var']['prpid']}
								AND iu.inuid = {$_SESSION['brasilpro_var']['inuid']}";
	
	//ver($sql);die;
	echo '
		<table id="listaSub100Empenhada" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<thead>
				<tr>
					<td>Lista de Subações 100% empenhadas do tipo Termo e 99% do tipo Convênio</td>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>
	';			
		$cabecalho = array("&nbsp;","Nº do Empenho","Subação", "% Empenhado", "Valor Emepnhado" );
		$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');

	echo '
					</td>
				</tr>
			</tbody>
		</table>
	';
		
	// Lista de Subações a serem empenhadas
	/*
	$sql = "
	SELECT DISTINCT dados.chk,
			dados.numeroempenho,
			dados.descricaosubacao,
			dados.valorsubacao,
			dados.percentualempenhado,
			dados.valorempenhado,
			dados.porcentagemaempenhar,
			CASE WHEN dados.percentualempenhado <> 0
				THEN (dados.valorsubacao -  dados.valorempenhado)::numeric
				ELSE (dados.valorsubacao * dados.porcentagem)::numeric / 100::numeric
			END AS valoraempenhar,
			CASE WHEN dados.percentualempenhado <> 0
				THEN '<input type=hidden id=vlrTotal_'||dados.sbaid||' name=vlrTotal_'||dados.sbaid||' value='|| (dados.valorsubacao -  dados.valorempenhado)::numeric ||'>'
				ELSE '<input type=hidden id=vlrTotal_'||dados.sbaid||' name=vlrTotal_'||dados.sbaid||' value='|| (dados.valorsubacao * dados.porcentagem)::numeric / 100::numeric ||'>'
			END AS valoraempenharT
	FROM (
			SELECT '<input type=checkbox name=chk[] onclick=marcarChk(this); id=chk_'||s.sbaid||' value='||s.sbaid||'>' as chk, 
					s.sbaid,
					e.empnumero as numeroempenho, 
					s.sbadsc as descricaosubacao, 
					CASE WHEN sbacronograma = 1 
						THEN ( SELECT sum(coalesce(sic.icoquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
							FROM cte.subacaoitenscomposicao sic 
							WHERE sic.sbaid = s.sbaid
							AND sic.icoano = sd.sbdano
							GROUP BY sic.sbaid )
						ELSE ( SELECT sum(coalesce(se.sesquantidade,0) * coalesce(sic.icovalor,0))::numeric as vlrsubacao
							FROM cte.subacaoitenscomposicao sic 
							INNER JOIN cte.subacaoescolas se ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
							WHERE sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano
							GROUP BY sic.sbaid ) 
					END AS valorsubacao,
					coalesce(es.eobpercentualemp,0) as percentualempenhado, 
					coalesce(es.eobvalorempenho,0) as valorempenhado,
					'<input type=text id=id_'||s.sbaid||' value='||
																	CASE WHEN p.prptipoexecucao = 'C' 
																		THEN (99 - coalesce(es.eobpercentualemp,0)) 
																		ELSE (100 - coalesce(es.eobpercentualemp,0)) 
																	END 
					||' name=name_'||s.sbaid||' size=6 onKeyUp=\"calculaEmpenho(this);\" onBlur=\"verificaPreenchimentoPorcentagem(this) \" class=\"disabled\" readonly=readonly onfocus=\"this.select();\">
					<input type=hidden id=vlr_'||s.sbaid||' name=vlr_'||s.sbaid||' value='||
																							CASE WHEN p.prptipoexecucao = 'C' 
																								THEN (99 - coalesce(es.eobpercentualemp,0)) 
																								ELSE (100 - coalesce(es.eobpercentualemp,0)) 
																							END
					
					||'>
					<input type=hidden id=anosubacao_'||sd.sbdano||' name=anosubacao_'||sd.sbdano||' value='||sd.sbdano||'>' 
					AS porcentagemaempenhar,
					CASE WHEN p.prptipoexecucao = 'C' 
							THEN (99 - coalesce(es.eobpercentualemp,0)) 
							ELSE (100 - coalesce(es.eobpercentualemp,0)) 
							END AS porcentagem
			FROM cte.instrumentounidade 	iu
			INNER JOIN cte.pontuacao 	po on po.inuid   = iu.inuid AND ptostatus <> 'I'
			INNER JOIN cte.acao 		a  on a.ptoid   = po.ptoid --AND acistatus = 'A'
			INNER JOIN cte.subacao		s  on s.aciid   = a.aciid AND sbastatus = 'A'
			INNER JOIN cte.processobrasilpro 	p  ON p.inuid = iu.inuid
			INNER JOIN cte.subacaodetalhe 	sd ON sd.sbaid = s.sbaid AND p.prpid = sd.prpid -- AND sd.sbdano = date_part('year', now()) 
			LEFT JOIN  cte.empenhosubacao   es ON es.sbaid = s.sbaid AND es.eobano = sd.sbdano 
			LEFT JOIN cte.empenho 		e  ON e.empid = es.empid 
			WHERE p.prpid = {$_SESSION['brasilpro_var']['prpid']}
			AND iu.inuid = {$_SESSION['brasilpro_var']['inuid']}
			AND sd.ssuid = 2 -- aprovada pela comissão 
		
			AND true = (	
							SELECT CASE WHEN pr.prptipoexecucao = 'C' 
									THEN 
										CASE WHEN (ems.eobpercentualemp IS NULL OR ems.eobpercentualemp < 99)
										THEN TRUE 
										ELSE FALSE END
									ELSE 
										CASE WHEN (ems.eobpercentualemp IS NULL OR ems.eobpercentualemp < 100)
										THEN TRUE 
										ELSE FALSE END
								END as d
							FROM cte.processobrasilpro pr
							LEFT JOIN cte.empenho e on e.empnumeroprocesso = pr.prpnumeroprocesso
							LEFT JOIN cte.empenhosubacao ems on ems.empid = e.empid
							WHERE  pr.prpid =  {$_SESSION['brasilpro_var']['prpid']}
							
	) 
			) AS dados";
	*/

	$sql = "SELECT DISTINCT 
			            dados.chk,			
			            dados.numeroempenho,			
			            dados.descricaosubacao,			
			            dados.valorsubacao,			
			            dados.percentualempenhado,			
			            dados.valorempenhado,			
			            dados.porcentagemaempenhar,			
			            CASE WHEN dados.percentualempenhado <> 0			
			                        THEN (dados.valorsubacao -  dados.valorempenhado)::numeric			
			                        ELSE (dados.valorsubacao * dados.porcentagem)::numeric / 100::numeric			
			            END AS valoraempenhar,
			            dados.sptano,			
			            CASE WHEN dados.percentualempenhado <> 0			
			                        THEN '<input type=hidden id=vlrTotal_'||dados.sbaid||' name=vlrTotal_'||dados.sbaid||' value='|| (dados.valorsubacao -  dados.valorempenhado)::numeric ||'>'
						ELSE '<input type=hidden id=vlrTotal_'||dados.sbaid||' name=vlrTotal_'||dados.sbaid||' value='|| (dados.valorsubacao * dados.porcentagem)::numeric / 100::numeric ||'>'			
			            END AS valoraempenharT
			         				
			FROM (			
			            SELECT           
			            			'<input type=checkbox name=chk[] onclick=marcarChk(this); id=chk_'||s.sbaid||' value='||s.sbaid||'>' as chk, 			
			                        s.sbaid,			
			                        e.empnumero as numeroempenho,			
			                        s.sbadsc as descricaosubacao,			
			                        CASE WHEN sbaporescola = false			
			                                   THEN			
			                                   (			
			                                               SELECT sum(coalesce(sic.cosqtd,0) * coalesce(sic.cosvlruni,0))::numeric as vlrsubacao			
			                                               FROM cte.composicaosubacao sic			
			                                               WHERE sic.sbaid = s.sbaid			
			                                               AND sic.cosano = sd.sptano			
			                                               GROUP BY sic.sbaid			
			                                   )			
			                                   ELSE			
			                                   (			
			                                               SELECT sum(coalesce(ecs.ecsqtd,0) * coalesce(sic.cosqtd,0))::numeric as vlrsubacao			
			                                               FROM cte.composicaosubacao sic			
			                                               INNER JOIN cte.escolacomposicaosubacao ecs ON sic.cosid = ecs.cosid			
			                                               WHERE sic.sbaid = s.sbaid  AND sic.cosano = sd.sptano			
			                                               GROUP BY sic.sbaid			
			                                   )			
			                        END AS valorsubacao,			
			                        coalesce(es.eobpercentualemp,0) as percentualempenhado,			
			                        coalesce(es.eobvalorempenho,0) as valorempenhado,	
			                      '<input type=text id=id_'||s.sbaid||' value='||
																	CASE WHEN p.prptipoexecucao = 'C' 
																		THEN (99 - coalesce(es.eobpercentualemp,0)) 
																		ELSE (100 - coalesce(es.eobpercentualemp,0)) 
																	END 
									||' name=name_'||s.sbaid||' size=6 onKeyUp=\"calculaEmpenho(this);\" onBlur=\"verificaPreenchimentoPorcentagem(this) \" class=\"disabled\" readonly=readonly onfocus=\"this.select();\">
									<input type=hidden id=vlr_'||s.sbaid||' name=vlr_'||s.sbaid||' value='||
																											CASE WHEN p.prptipoexecucao = 'C' 
																												THEN (99 - coalesce(es.eobpercentualemp,0)) 
																												ELSE (100 - coalesce(es.eobpercentualemp,0)) 
																											END
									
									||'>
									<input type=hidden id=anosubacao_'||s.sbaid||' name=anosubacao_'|| s.sbaid || ' value='||sd.sptano||'>' 
									AS porcentagemaempenhar,		
			                        CASE WHEN p.prptipoexecucao = 'C'			
			                            THEN (99 - coalesce(es.eobpercentualemp,0))			
			                            ELSE (100 - coalesce(es.eobpercentualemp,0))			
			                        END AS porcentagem,
			                        sd.sptano			
			            FROM cte.instrumentounidade               iu			
			            INNER JOIN cte.pontuacao                    po on po.inuid   = iu.inuid AND ptostatus = 'A'			
			            INNER JOIN cte.acaoindicador               a  on a.ptoid   = po.ptoid			
			            INNER JOIN cte.subacaoindicador         s  on s.aciid   = a.aciid --AND sbastatus = 'A'			
			            INNER JOIN cte.processobrasilpro         p  ON p.inuid = iu.inuid			
			            INNER JOIN cte.subacaoparecertecnico             sd ON sd.sbaid = s.sbaid AND p.prpid = sd.prpid -- AND sd.sbdano = date_part('year', now())			
			            LEFT JOIN  cte.empenhosubacao          es ON es.sbaid = s.sbaid --AND es.eobano = sd.sbdano			
			            LEFT JOIN cte.empenho                       e  ON e.empid = es.empid
			            			
			            WHERE p.prpid = {$_SESSION['brasilpro_var']['prpid']}			
			            AND iu.inuid = {$_SESSION['brasilpro_var']['inuid']}	

			            AND sd.ssuid = 3 -- aprovada pela comissão			
			            AND true = (   			
			                            SELECT CASE WHEN pr.prptipoexecucao = 'C'			
			                                       THEN			
			                                               CASE WHEN (ems.eobpercentualemp IS NULL OR ems.eobpercentualemp < 99)			
			                                               THEN TRUE			
			                                               ELSE FALSE END			
			                                       ELSE			
			                                               CASE WHEN (ems.eobpercentualemp IS NULL OR ems.eobpercentualemp < 100)			
			                                               THEN TRUE			
			                                               ELSE FALSE END			
			                                   END as d			
			                            FROM cte.processobrasilpro pr			
			                            LEFT JOIN cte.empenho e on e.empnumeroprocesso = pr.prpnumeroprocesso			
			                            LEFT JOIN cte.empenhosubacao ems on ems.empid = e.empid			
			                            WHERE  pr.prpid =  {$_SESSION['brasilpro_var']['prpid']}
			            )			
		            ) AS dados";
	
	//ver($sql);die;
	echo '
	
		<table id="listaSubASeremEmpenhada" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<thead>
				<tr>
					<td>Lista de Subações a serem Empenhadas</td>
				</tr>
			</thead>
			<tbody>
				<tr>
					<td>
	';
		$cabecalho = array("&nbsp;","Nº do Empenho","Subação","Valor da Subação", "% Empenhado", "Valor Empenhado", "% a Empenhar", "Valor a Empenhar" , "Ano" );
		$db->monta_lista_simples($sql,$cabecalho,500,5,'S','100%','S');
	
	echo '			</td>
				</tr>
			</tbody>
		</table>
	';
}

function listaHistoricoEmpenho($dados) {
	global $db;
	$sql = "SELECT u.usunome, to_char(hepdata, 'dd/mm/YYYY HH24:MI') as data, empsituacao, ds_problema, valor_total_empenhado, valor_saldo_pagamento FROM cte.historicoempenho h
			LEFT JOIN seguranca.usuario u ON u.usucpf=h.usucpf
			WHERE h.empid='".$dados['empid']."'";
	
	$cabecalho = array("Usuário atualização","Data","Situação","Problema encontrado","Valor empenhado(R$)","Valor pagamento(R$)");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);

}

function listaEmpenhoProcesso($dados) {
	global $db;

		if($dados['empnumeroprocesso']) {
			$where[] = "empnumeroprocesso='".$dados['empnumeroprocesso']."'";
		} elseif($_SESSION['brasilpro_var']['prpid']) {
			$where[] = "prpid='".$_SESSION['brasilpro_var']['prpid']."'";
		}
		$where[] = "funid = 1";

		$sql = "SELECT '<img align=absmiddle src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarHistoricoEmpenho(\''||e.empid||'\', this);\">' as mais,
					   CASE WHEN e.empsituacao!='CANCELADO' THEN '<img src=../imagens/refresh2.gif style=cursor:pointer; onclick=consultarEmpenho('||e.empid||',\'' || trim(e.empnumeroprocesso) || '\');>' ELSE '&nbsp;' END as acao_consultar,
					   CASE WHEN e.empsituacao!='CANCELADO' THEN '<img src=../imagens/excluir.gif align=absmiddle style=cursor:pointer; onclick=cancelarEmpenho('||e.empid||',\'' || trim(e.empnumeroprocesso) || '\');>' ELSE '&nbsp;' END as acao_cancelar,
					   e.empcnpj, en.entnome, e.empprotocolo, e.empnumero, e.empvalorempenho, u.usunome, e.empsituacao 
				FROM cte.empenho e
				LEFT JOIN cte.processobrasilpro p ON trim(e.empnumeroprocesso) = trim(p.prpnumeroprocesso)
				LEFT JOIN seguranca.usuario u ON u.usucpf=e.usucpf
				LEFT JOIN entidade.entidade en ON en.entnumcpfcnpj=e.empcnpj
				LEFT JOIN entidade.funcaoentidade fun ON fun.entid=en.entid
				".(($where)?"WHERE ".implode(" AND ", $where):"");

	$cabecalho = array("&nbsp;","&nbsp;","&nbsp;","CNPJ","Entidade","Nº protocolo","Nº empenho","Valor empenho(R$)","Usuário criação","Situação empenho");
	$db->monta_lista_simples($sql,$cabecalho,500,5,'N','100%',$par2);
}


function consultarEmpenho($dados) {
	global $db;

	try {

		$data_created = date("c");

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$usuario = 'MECTIAGOT';
			$senha   = 'M3135689';
			$nu_seq_ne = "73289";
		} else {
			$usuario = $dados['wsusuario'];
			$senha   = $dados['wssenha'];

		    $dadosemp = $db->pegaLinha("SELECT * FROM cte.empenho WHERE empid='".$dados['empid']."'");

	        if($dadosemp) {
	        	$nu_seq_ne = $dadosemp['empprotocolo'];
	        }


		}

    	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_seq_ne>$nu_seq_ne</nu_seq_ne>
		</params>
	</body>
</request>
XML;

		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
				->setURL($urlWS)
				->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
				->execute();

		$xmlRetorno = $xml;

	    $xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;
	    
		if($result) $hwpwebservice="consultarEmpenho - Sucesso";
		else $hwpwebservice="consultarEmpenho - Erro";
		
		$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
			    	prpid,
			    	hwpwebservice,
			    	hwpxmlenvio,
			    	hwpxmlretorno,
			    	hwpdataenvio,
			        usucpf)
			    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
			    		'".$hwpwebservice."',
			    		'".addslashes($arqXml)."',
			    		'".addslashes($xmlRetorno)."',
			    		NOW(),
			            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();

		
	    echo "------ CONSULTA DE EMPENHO ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
		echo "*** Detalhes da consulta ***\n\n";
		echo "* Nº processo : ".(($xml->body->row->processo)?$xml->body->row->processo:"-")."\n";
		echo "* CNPJ : ".$xml->body->row->nu_cnpj."\n";
		echo "* Valor(R$) : ".number_format($xml->body->row->valor_ne,2,",",".")."\n";
		echo "* Data : ".$xml->body->row->data_documento."\n";
		echo "* Nº documento : ".((strlen($xml->body->row->numero_documento))?$xml->body->row->numero_documento:"-")."\n";
		echo "* Valor empenhado(R$) : ".((strlen($xml->body->row->valor_total_empenhado))?$xml->body->row->valor_total_empenhado:"-")."\n";
		echo "* Saldo pagamento(R$) : ".((strlen($xml->body->row->valor_saldo_pagamento))?$xml->body->row->valor_saldo_pagamento:"-")."\n";
		echo "* Situação : ".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."\n\n";
		
		if($xml->body->row->numero_documento) $set[] = "empnumero='".$xml->body->row->numero_documento."'";
		if($xml->body->row->ds_problema) $set[] = "ds_problema='".$xml->body->row->ds_problema."'";
		if($xml->body->row->valor_total_empenhado) $set[] = "valor_total_empenhado='".$xml->body->row->valor_total_empenhado."'";
		if($xml->body->row->valor_saldo_pagamento) $set[] = "valor_saldo_pagamento='".$xml->body->row->valor_saldo_pagamento."'";
		if($xml->body->row->situacao_documento) $set[] = "empsituacao='".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."'";
		
		if($set) {
			$db->executar("UPDATE cte.empenho SET ".(($set)?implode(",",$set):"")." 
						   WHERE empid='".$dados['empid']."'");
		}

		$sql = "INSERT INTO cte.historicoempenho(
           		usucpf, empid, hepdata, empsituacao, ds_problema, valor_total_empenhado,
            	valor_saldo_pagamento)
    			VALUES ('".$_SESSION['usucpf']."',
    					'".$dados['empid']."',
    					NOW(),
    					'".iconv("UTF-8", "ISO-8859-1", $xml->body->row->situacao_documento)."',
    					'".$xml->body->row->ds_problema."',
    					".((strlen($xml->body->row->valor_total_empenhado))?"'".$xml->body->row->valor_total_empenhado."'":"NULL").",
    					".((strlen($xml->body->row->valor_saldo_pagamento))?"'".$xml->body->row->valor_saldo_pagamento."'":"NULL").");";

		$db->executar($sql);
		$db->commit();
			
		if($result) {
			return false;
		} else {
		   	return true;
		}


	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			echo "Erro-Serviço Consulta empenho encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		echo "Erro-WS Consultar empenho no SIGEF: $erroMSG";


	}
}

function solicitarConvenio($dados){
	global $db;		

	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	$xmlRetorno = "";

	$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prpnumeroconveniofnde, prpanoconveniofnde FROM cte.processobrasilpro WHERE prpid='".$_SESSION['brasilpro_var']['prpid']."'");
	
	if($dadosse){
		$nu_cnpj_favorecido=$db->pegaUm("SELECT ent.entnumcpfcnpj
					 				   FROM entidade.entidade ent
					 				   INNER JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
					 				   INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
					 				   WHERE fen.funid=1 AND ende.muncod='".$dadosse['muncod']."'");
		
	}

	$anoAtual     = date('Y');
	$dataAtualXml = date("c");
	
	if($nu_cnpj_favorecido){
			$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
<header>
<app>string</app>
<version>string</version>
<created>$dataAtualXml</created>
</header>
	<body>
	<params>
		<co_programa_fnde>03</co_programa_fnde>
		<an_exercicio>$anoAtual</an_exercicio>
	</params>
	</body>
</request>
XML;

		//if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		//	$urlWS = "http://172.20.200.116/webservices/sigefemendas/integracao/web/dev.php/convenio/consultar";						
		//} else {
			$urlWS = "http://www.fnde.gov.br/webservices/sigefemendas/index.php/convenio/consultar";
		//}
		try {

			$xsd = APPRAIZ.'emenda/modulos/sistema/ws-sigef/xsd/xml.fnde.ws-sigef.convenio.consultar.request.xsd';
			$arMessage = validaXML( $arqXml, $xsd );
			
		    if( empty( $arMessage ) ){
		    	$xml = Fnde_Webservice_Client::CreateRequest()
					->setURL($urlWS)
					->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
					->execute();
		    } else {
		    	echo 'Erro-WS: validação xml - Consulta Convenio: <br>';
		    	foreach ($arMessage as $msg) {
		    		print_r( $msg['text'] );
		    	}
		    	die;
		    }
		//echo $xml;
		//echo '-------------';
			$xmlRetorno = $xml;
			$xml = simplexml_load_string( stripslashes($xml));

			if ( (int) $xml->status->result ){
		        $obConvenio = $xml->body->children();

		        $nu_convenio  = (int) $obConvenio->nu_convenio;
		        $an_exercicio = (int) $obConvenio->an_exercicio;

				if($nu_convenio && $an_exercicio){
					# Atualiza a tabela de processopar.
					$sql = "update cte.processobrasilpro set prpnumeroconveniofnde = {$nu_convenio}, prpanoconveniofnde =  {$an_exercicio} where prpid = '".$_SESSION['brasilpro_var']['prpid']."'";
					$db->executar($sql);
			
				} else {
					throw new Exception('Não foi possível recuperar Número e o Ano do convênio.');
				}
		    } else {
		    	throw new Exception(utf8_decode($xml->status->error->message->text));
		    }

		} catch (Exception $e){
			/**
			 * @var LogErroWS
			 * Bloco que grava o erro em nossa base
			 */

			$sql = "INSERT INTO cte.historicowsprocessobrasilpro(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['brasilpro_var']['prpid']."',
				    		'solicitarConvenio - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";
	
			$db->executar($sql);
			$db->commit();
			echo "\n\n------ CONSULTAR NÚMERO DE CONVÊNIO ------\n\n";
			
			# Erro 404 página not found
			if($e->getCode() == 404){
				echo "Erro-Serviço Convênio encontra-se temporariamente indisponível.\nFavor tente mais tarde.";
				return false;
				die;
			}
			
			echo 'Erro-WS Consulta Convenio: '.$e->getMessage();
			return false;
			die;
		}	
	}
	$db->commit();
	return $obConvenio;
}

?>
