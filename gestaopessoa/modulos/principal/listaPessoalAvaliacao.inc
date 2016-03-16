<?php 
if( $_POST['ajaxcpf'] != '' ){
	$_SESSION['boautoavaliacao'] = false;
	$_SESSION['cpfavaliado'] = $_POST['ajaxcpf']; 
	$_SESSION['autoavalchefe'] = false;
	$dados = verificaDadosPesFunc($_POST['ajaxcpf']);
	die();
}elseif( $_POST['ajaxcpfautoaval'] != '' ){
	$_SESSION['boautoavaliacao'] = true;
	$_SESSION['cpfavaliado'] = $_POST['ajaxcpfautoaval']; 
	$_SESSION['autoavalchefe'] = true;
	$dados = verificaDadosPesFunc($_POST['ajaxcpfautoaval']);
	die();
}
 
$_SESSION['boautoavaliacao'] = false;
$_SESSION['autoavalchefe'] = false;
//direcionaAvaliador($_SESSION['usucpf']);
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
monta_titulo( 'Avaliação - GDPGPE', 'Lista de Pessoas' ); 

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form action="" method="POST" name="formulario">
					<input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>
						<div style="float: left;">
							<table border="0" cellpadding="3" cellspacing="0">
								<tr>
									<td valign="bottom" >
										Nome da Pessoa:
										<br/> 
										<?
										$filtro_nome = $_POST['filtro_nome'];
										echo campo_texto( 'filtro_nome', 'N', 'S', '', 60, 200, '', '' ); ?>
									</td>
									<td  valign="bottom" >
									 	CPF:
										<br/> 
										<?
										$filtro_cpf = $_POST['filtro_cpf'];
										echo campo_texto( 'filtro_cpf', 'N', 'S', '', 20, 200, '', '' ); ?>
									</td>
									<td  valign="bottom" >
									 	SIAPE:
										<br/> 
										<?
										$filtro_siape = $_POST['filtro_siape'];
										echo campo_texto( 'filtro_siape', 'N', 'S', '', 30, 200, '', '' ); ?>
									</td>
									<td valign="bottom" >
										<input type="button" name="" value="Pesquisar" onclick="return validaForm();"/>
									</td>
								</tr>
							</table>
						</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>	
<div id="debug"></div>
<?php listaEquipeAvaliacao($_SESSION['usucpf']); ?>

<script type="text/javascript">
function validaForm(){
	document.formulario.submit();
}

function selecionarPessoa(cpf){
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/listaPessoalAvaliacao&acao=A', {
						        method:     'post',
						        parameters: '&ajaxcpf=' + cpf,							         
						        onComplete: function (res) {
						        	if(res.responseText == 1){
						        		if( confirm('Confirma a revisão dos Dados Pessoais e Funcionais? ') ){
											window.location.href = '?modulo=principal/formularioAvaliacao&acao=A';
						        		}
									} else {
										alert('Dados pessoais e funcionais não informados. Favor acessar a aba FORÇA DE TRABALHO na página inicial!');
									}
								}
			}); 
}

function selecionarPessoaAutoAval(cpf){
	var req = new Ajax.Request('gestaopessoa.php?modulo=principal/listaPessoalAvaliacao&acao=A', {
						        method:     'post',
						        parameters: '&ajaxcpfautoaval=' + cpf,							         
						        onComplete: function (res) { 
						        	//$('debug').innerHTML = res.responseText;
						        	if(res.responseText == 1){
						        		if( confirm('Confirma a revisão dos Dados Pessoais e Funcionais? ') ){
											window.location.href = '?modulo=principal/formularioAvaliacao&acao=A';
						        		}
						        	} else {
										alert('Dados pessoais e funcionais não informados. Favor acessar a aba FORÇA DE TRABALHO na página inicial!');
									}
								}
			});
}
 
</script>