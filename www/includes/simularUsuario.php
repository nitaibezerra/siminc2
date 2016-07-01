<?php
header('content-type: text/html; charset=ISO-8859-1');
set_time_limit(30000);
ini_set("memory_limit", "3000M");

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

if($_REQUEST['requisicaoajax'] == 'carregarlista'){
	try{
		if ($_REQUEST['usunome']) 		 $where = " and TRANSLATE(u.usunome, 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ilike TRANSLATE('%".$_REQUEST['usunome']."%', 'áàãâéêíóôõúüçÁÀÃÂÉÊÍÓÔÕÚÜÇ','aaaaeeiooouucAAAAEEIOOOUUC') ";
		if ($_REQUEST['usucpf']) 		 $where.= " and u.usucpf like '%".str_to_upper(corrige_cpf($_REQUEST['usucpf']))."%'";
		if ($_REQUEST['pflcod']) 		 $where.= " and p.pflcod = ".$_REQUEST['pflcod'];
		
		$sql = "select distinct
					case when u.usucpf = '".$_SESSION['usucpf']."' then
						'<center><input type=\"radio\" checked disabled name=\"chk\" id=\"chk\" value=\"'||u.usucpf||'\"></center>'
					else 
						'<center><input type=\"radio\" name=\"chk\" id=\"chk\" value=\"'||u.usucpf||'\" onclick=\"selecionaUsuario(\''||u.usucpf||'\');\"></center>' 
					end as acoes,
					u.usucpf as codigo,
					u.usunome ||' - <span style=\"color: red\"><b>Usuário Origem.</b></span>' as descricao
				from seguranca.usuario u
				where u.usucpf = '{$_SESSION['usucpforigem']}'";
		
		$arrUsuarioAtual = $db->pegaLinha($sql);
		$arrUsuarioAtual = $arrUsuarioAtual ? $arrUsuarioAtual : array();
		
		$sql = "select distinct
				'<center><input type=\"radio\" name=\"chk\" id=\"chk\" value=\"'||u.usucpf||'\" onclick=\"selecionaUsuario(\''||u.usucpf||'\');\"></center>' as acoes,
				u.usucpf as codigo,
				u.usunome as descricao
				from seguranca.usuario u
				inner join seguranca.perfilusuario pf on pf.usucpf = u.usucpf
				inner join perfil p on p.pflcod = pf.pflcod and	p.sisid = " . $_SESSION['sisid'] . "
				inner join seguranca.usuario_sistema us on us.usucpf = u.usucpf and	us.sisid = p.sisid
				where us.suscod = 'A' and p.pflnivel >= (
						select min(pflnivel) from seguranca.perfil
						inner join seguranca.perfilusuario on perfil.pflcod = perfilusuario.pflcod
						where perfilusuario.usucpf = '" . $_SESSION['usucpforigem'] . "'
						and perfil.sisid = " . $_SESSION['sisid'] . " )
						and u.usucpf not in ('{$_SESSION['usucpforigem']}')
						$where
				order by u.usunome ";
		
		$arrUsuarioS = $db->carregar($sql);
		$arrUsuarioS = $arrUsuarioS ? $arrUsuarioS : array();
		
		$arrDados = array_merge(array($arrUsuarioAtual),  $arrUsuarioS);
					
		$cabecalho = array( 'Ação', 'CPF', 'Nome Completo' );
		return $db->monta_lista_simples($arrDados, $cabecalho, 1000000, 20, 'N', '100%');
		
	} catch (Exception $e) {
        echo ($e->getMessage());
    }
    die;
}

monta_titulo('Simular Usuário: '.$_SESSION['sisdsc'], '');
$usunome = $_REQUEST['usunome'];
$usucpf = $_REQUEST['usucpf'];

?>
<html>
	<head>
		<style>
		#loader-container,
		#LOADER-CONTAINER{
		    background: transparent;
		    position: absolute;
		    width: 500px;
		    text-align: center;
		    z-index: 8000;
		    height: 300px;
		}
		
		
		#loader {
		    background-color: #fff;
		    color: #000033;
		    width: 250px;
		    border: 2px solid #cccccc;
		    font-size: 12px;
		    font-weight: bold;
		    padding: 25px; 
		    margin-top: 10%;
		    margin-left: 20%;
		}
		</style>
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript">
			$(document).ready(function(){
				window.document.getElementById('usunome').focus();
			});
	
			function fecharUsuarioSimular(){
				//closeMessage();
				window.close();
			}
				
			function selecionaUsuario(usucpf){
				window.opener.document.getElementById('usrs').value = usucpf;
				window.opener.setpfl(usucpf);
				window.close();
			}
			
			function enviarUsuarioSimular(){
				document.getElementById('btnPesquisarUsuarioSimula').disabled = true;
			    var ajax = window.XMLHttpRequest ? new XMLHttpRequest : new window.ActiveXObject('Microsoft.XmlHttp');
			        
			    var usucpf = document.getElementById('usucpf').value;
			    var usunome = document.getElementById('usunome').value;
			    var pflcod = document.getElementById('pflcod').value;
			    
			    if( usucpf == '' && usunome == '' && pflcod == ''){
					alert('O campos não podem ser vazios!');
					document.getElementById('btnPesquisarUsuarioSimula').disabled = false;
					return false;
				}
				
				var url = '?requisicaoajax=carregarlista&usucpf='+usucpf+'&usunome='+usunome+'&pflcod='+pflcod;
			
			    ajax.onreadystatechange = function(){
			
			        if (ajax.readyState != 4) return;
					
			        document.getElementById('listaUsuario').innerHTML = ajax.responseText;
			        
			        document.getElementById('btnPesquisarUsuarioSimula').disabled = false;
			    };
				
				ajax.open('POST', '../includes/simularUsuario.php'+url, true);
			    ajax.send(null);
			}
		</script>
	</head>
<body>
<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
<form name="formulario" id="formulario" method="post">
<table id="tblform" class="tabela" width="100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
	<tr>
		<td align='right' class="SubTituloDireita">CPF (ou parte do CPF):</td>
		<td><?=campo_texto('usucpf','','','',16,14,'###.###.###-##','', '', '', '', 'id="usucpf"', '', '', "this.value=mascaraglobal('###.###.###-##',this.value);");?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome completo (ou parte do nome):</td>
		<td><?=campo_texto('usunome','','','',35,50,'','', '', '', '', 'id="usunome"');?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Perfil:</td>
		<td><? 
		$sql = "SELECT
					 pflcod AS codigo,
					 pfldsc AS descricao
				FROM
					 seguranca.perfil
				WHERE
					 pflstatus = 'A' and sisid = ".$_SESSION['sisid']."
				order by
					pfldsc";		
		$db->monta_combo("pflcod", $sql, 'S', 'Todos', '', '', '', '', '', 'pflcod' );
		?></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center">
			<input type="hidden" name="requisicao" id="requisicao" value="">
			<input type="button" value="Pesquisar" name="btnPesquisarUsuarioSimula" id="btnPesquisarUsuarioSimula" style="cursor: pointer;" onclick="enviarUsuarioSimular();">
			<input type="button" value="Fechar" name="btnFechar" id="btnFechar" style="cursor: pointer;" onclick="fecharUsuarioSimular();">
		</td>
	</tr>
	<tr>
		<td colspan="2">
		<div style="overflow-x: auto; overflow-y: auto; width:100%; height:160px; text-align: center;">
			<div id="listaUsuario">
			<?
			$sql = "select distinct
						case when u.usucpf = '".$_SESSION['usucpf']."' then
							'<center><input type=\"radio\" checked disabled name=\"chk\" id=\"chk\" value=\"'||u.usucpf||'\"></center>'
						else 
							'<center><input type=\"radio\" name=\"chk\" id=\"chk\" value=\"'||u.usucpf||'\" onclick=\"selecionaUsuario(\''||u.usucpf||'\');\"></center>' 
						end as acoes,
						u.usucpf as codigo,
						u.usunome ||' - <span style=\"color: red\"><b>Usuário Origem.</b></span>' as descricao
					from seguranca.usuario u
					where u.usucpf = '{$_SESSION['usucpforigem']}'";

			$arrUsuarioAtual = $db->carregar($sql);
			$arrUsuarioAtual = $arrUsuarioAtual ? $arrUsuarioAtual : array();
			
			$cabecalho = array( 'Ação', 'CPF', 'Nome Completo' );
			$db->monta_lista_simples($arrUsuarioAtual, $cabecalho, 1000000, 20, 'N', '100%');
			?>
			</div>
		</div>	
		</td>
	</tr>
</table>
</form>
<div id="lista"></div>
</body>
</html>