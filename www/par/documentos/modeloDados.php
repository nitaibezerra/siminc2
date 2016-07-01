<?php 
include_once 'config.inc';
include APPRAIZ . '/includes/funcoes.inc';
include APPRAIZ . '/includes/classes_simec.inc';
require_once APPRAIZ . "www/includes/webservice/cpf.php";

include APPRAIZ . 'www/par/autoload.php';
include APPRAIZ . 'www/par/_constantes.php';

$obPreObraControle = new PreObraControle();
$arDados = $obPreObraControle->recuperarPrefeitoMunicipio($_SESSION['par']['muncod']);
$arDadosPrefeitura = $obPreObraControle->recuperarPrefeitoMunicipio($_SESSION['par']['muncod'], 1);

//ver($arDados, $arDadosPrefeitura);
//$arDadosPrefeitura['sede'] = $arDadosPrefeitura['entsede'] ? $arDadosPrefeitura['endlog'].", ".$arDadosPrefeitura['endbai'] : "______________________";
?>
<html>
	<head>
		<title>PAR - Cadastro de Itens Composição</title>
		<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css" />
		<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
		<script type="text/javascript" src="../../includes/funcoes.js" ></script>	
		<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>		
		<script type="text/javascript" src="/includes/prototype.js"></script>
		<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
		<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
		<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
		<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
		<link rel="stylesheet" href="/includes/entidadesn.css" type="text/css" media="screen" />
		<script type="text/javascript" src="/includes/entidadesn.js"></script>
				
		<script type="text/javascript">
			jQuery.noConflict();
			
			jQuery(document).ready(function(){
				jQuery('.mesclar').click(function(){
		
					if(jQuery('input[name=duncpf]').val() == ''){
						alert('O campo cpf é obrigatório!');
						jQuery('input[name=duncpf]').focus();
						return false;
					}
		
					if(jQuery('input[name=crea]').val() == ''){
						alert('O campo crea é obrigatório!');
						jQuery('input[name=crea]').focus();
						return false;
					}
		
					document.formulario.submit();
				});

				jQuery('input.classcpf').live('change',function(){
					if( !validar_cpf( jQuery(this).val()  ) ){
						alert( "CPF inválido!\nFavor informar um cpf válido!" );
						jQuery(this).val('');
						return false;	
					}
					
					var comp  = new dCPF();
					var input_entnome = jQuery('#input_entnome');
					var label_entnome = jQuery('#label_entnome');
										
					comp.buscarDados( jQuery(this).val() );
					if (comp.dados.no_pessoa_rf != ''){
						input_entnome.val(comp.dados.no_pessoa_rf);
						label_entnome.html(comp.dados.no_pessoa_rf);
						input_entnome.attr("readonly","readonly");
					}

				});
			});
		</script>
	</head>
	<body>
		<center>			
			<?php monta_titulo('INFORME OS DADOS PARA MESCLAR COM O MODELO', '<img src="../../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.')?>
			<br />		
			<form action="modelo.php?modelo=<?php echo $_GET['modelo'] ?>" name="formulario" method="post">
				<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<?php if(empty($arDados['entnome']) && $_REQUEST['modelo'] != 8): ?>
						<tr>
							<td class="subtituloEsquerda"><label>Nome do prefeito: </label></td>
							<td>
								<?php echo campo_texto('entnomeprefeito', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					<?php if(empty($arDados['entnumcpfcnpj']) && $_REQUEST['modelo'] != 8): ?>
						<tr>
							<td class="subtituloEsquerda"><label>CPF do Prefeito: </label></td>
							<td>
								<?php echo campo_texto('entnumcpfcnpjprefeito', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					<?php if(empty($arDados['mundescricao']) && $_REQUEST['modelo'] != 8): ?>
						<tr>
							<td class="subtituloEsquerda"><label>Município: </label></td>
							<td>
								<?php echo campo_texto('mundescricaoprefeito', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					<?php if(empty($arDados['estdescricao']) && $_REQUEST['modelo'] != 8): ?>
						<tr>
							<td class="subtituloEsquerda"><label>UF: </label></td>
							<td>
								<?php echo campo_texto('estdescricaoprefeito', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					<?php if(empty($arDados['entnumrg']) && $_REQUEST['modelo'] != 8): ?>
						<tr>
							<td class="subtituloEsquerda"><label>N.º do RG: </label></td>
							<td>
								<?php echo campo_texto('entnumrgprefeito', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					<?php if(empty($arDados['endlog']) && $_REQUEST['modelo'] != 8): ?>
						<tr>
							<td class="subtituloEsquerda"><label>Endereço do prefeito: </label></td>
							<td>
								<?php echo campo_texto('endlogprefeito', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					<?php if(empty($arDadosPrefeitura['endlog']) && $_REQUEST['modelo'] != 8): ?>
						<tr>
							<td class="subtituloEsquerda"><label>Endereço da prefeitura: </label></td>
							<td>
								<?php echo campo_texto('endlogprefeitura', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					<?php if(empty($arDadosPrefeitura['entnumcpfcnpj']) && $_REQUEST['modelo'] != 8): ?>
						<tr>
							<td class="subtituloEsquerda"><label>CNPJ da prefeitura: </label></td>
							<td>
								<?php echo campo_texto('entnumcpfcnpjprefeitura', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					<?php if(empty($arDadosPrefeitura['endbai']) && $_REQUEST['modelo'] != 8): ?>
						<tr>
							<td class="subtituloEsquerda"><label>Bairro da prefeitura: </label></td>
							<td>
								<?php echo campo_texto('endbaiprefeitura', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					<?php if(empty($arDadosPrefeitura['entsede']) && $_REQUEST['modelo'] != 8): ?>
						<tr>
							<td class="subtituloEsquerda"><label>Sede da prefeitura: </label></td>
							<td>
								<?php echo campo_texto('entsedeprefeitura', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					
					<?php if($_REQUEST['modelo'] == 8): ?>
						<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245);" id="linha_1">
							<td class="subtituloEsquerda"><label>CPF: </label></td>
							<td>
								<input onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" class="normal classcpf" value="" size="31" name="duncpf" type="text" />
								<img title="Indica campo obrigatório." src="../../imagens/obrig.gif">
							</td>						
						</tr>
						<tr id="2">
							<td class="subtituloEsquerda"><label>Nome: </label></td>
							<td id="3">
								<input value="" id="input_entnome" name="dunnome" type="hidden" /><label id="label_entnome"></label>
							</td>
						</tr>
						<tr>
							<td class="subtituloEsquerda"><label>CREA: </label></td>
							<td>
								<?php echo campo_texto('crea', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					
					<?php if($_REQUEST['modelo'] == 9): ?>
						<tr>
							<td class="subtituloEsquerda"><label>Estado civil do prefeito: </label></td>
							<td>
								<?php echo campo_texto('estadocivil', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
						<tr>
							<td class="subtituloEsquerda"><label>Naturalidade do prefeito: </label></td>
							<td>
								<?php echo campo_texto('naturalidade', 'N', 'S', '', '30', '', '', '') ?>
								<img src="../../imagens/obrig.gif" border="0">
							</td>
						</tr>
					<?php endif; ?>
					<tr>
						<td colspan="2" align="left">
							<input type="button" value="Gerar documento" class="mesclar">
						</td>
					</tr>
				</table>
			</form>
		</center>
	</body>
</html>