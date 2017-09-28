<?php
 
extract($_GET);

if( $pesid )
{
	$sql = "SELECT
				p.pesnome,
				p.usucpf,
				d.fgtid,
				d.dpetelefone,
				d.dpeemail
			FROM
				pdeinterativo2013.pessoa p
			INNER JOIN
				pdeinterativo2013.detalhepessoa d ON d.pesid = p.pesid
			WHERE
				p.pesid = ".$pesid;
	$dadosMembro = $db->carregar($sql);
	
	extract($dadosMembro[0]);
	
	$dpecpf			= substr($usucpf, 0, 3).'.'.substr($usucpf, 3, 3).'.'.substr($usucpf, 6, 3).'-'.substr($usucpf, 9);
	$dpeddd			= substr($dpetelefone, 0, 2);
	$dpetelefone 	= substr($dpetelefone, 2);
	$dpetelefone	= substr($dpetelefone, 0, 4).'-'.substr($dpetelefone, 4);
}

?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
		<link rel="stylesheet" href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />
		<script type="text/javascript" src="/includes/JQuery/jquery-ui-1.8.4.custom.min.js"></script>
		<script type=text/javascript src=/includes/prototype.js></script>
		<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
		
		<style>
		.ui-widget
		{
			font-size: 10px;
		}
		</style>
		
		<script type="text/javascript">
		jQuery(document).ready(function()
		{
			jQuery( "#dialog" ).dialog(
			{
				autoOpen: false,
				resizable: false,
				height:170,
				modal: true,
				buttons: {
					"Sim": function()
					{
						jQuery('#pesnome').val('');
						jQuery('#dpecpf').val('');
						jQuery('#fgtid').val('');
						jQuery('#dpeddd').val('');
						jQuery('#dpetelefone').val('');
						jQuery('#dpeemail').val('');

						jQuery( this ).dialog( "close" );
					},
					"Não": function()
					{
						self.close();
					}
				}
			});
			
			jQuery('#btSalvar').click(function()
			{
				if( jQuery('#pesnome').val() == '' )
				{
					alert("O campo 'Nome' deve ser preenchido.");
					jQuery('#pesnome').focus();
					return;
				}
				if( jQuery('#dpecpf').val() == '' )
				{
					alert("O campo 'CPF' deve ser preenchido.");
					jQuery('#dpecpf').focus();
					return;
				}
				if( !validar_cpf( jQuery('#dpecpf').val() ) )
				{
					alert("O campo 'CPF' possui um valor com formato inválido.");
					jQuery('#dpecpf').focus();
					return;
				}
				if( jQuery('#fgtid').val() == '' )
				{
					alert("O campo 'Perfil' deve ser preenchido.");
					jQuery('#fgtdesc').focus();
					return;
				}
				if( jQuery('#dpeddd').val() == '' )
				{
					alert("O campo 'DDD' deve ser preenchido.");
					jQuery('#dpeddd').focus();
					return;
				}
				if( jQuery('#dpetelefone').val() == '' )
				{
					alert("O campo 'Telefone' deve ser preenchido.");
					jQuery('#dpetelefone').focus();
					return;
				}
				if( jQuery('#dpeemail').val() == '' )
				{
					alert("O campo 'Email' deve ser preenchido.");
					jQuery('#dpeemail').focus();
					return;
				}
				if( !validaEmail( jQuery('#dpeemail').val() ) )
				{
					alert("O campo 'Email' deve conter um endereço de email válido.");
					jQuery('#dpeemail').focus();
					return;
				}

				var existeMembro 	= false;
				var numeroMembros	= 0;
				
				if( jQuery('#pesid').val() == '' )
				{
					jQuery.ajax({
						   type: "post",
						   url: "pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A",
						   async: false,
						   dataType: 'json',
						   data: "ajaxExisteMembro=1&grtid="+jQuery('#grtid').val()+"&dpecpf="+jQuery('#dpecpf').val()+"",
						   success: function(msg)
						   {
							  if( msg.retorno )
							  {
								 existeMembro = true;
							  }

							  numeroMembros = msg.nummembros;
						   }
					});
				}

				if( existeMembro )
				{
					alert("O CPF informado já se encontra cadastrado para este Grupo de Trabalho.");
					jQuery('#dpecpf').focus();
					return;
				}
				
				if( Number(numeroMembros) == 10 )
				{
					alert("O número de membros do Grupo de Trabalho já chegou ao limite.");
					return;
				}
				
				/*var linha = '<tr>' +
								'<td align="center"><img style="cursor:pointer;" title="Altera as informações do membro do GT" src="/imagens/check_p.gif" border="0" onclick="alteraMembro(this.parentNode.parentNode.rowIndex);" />&nbsp;<img title="Exclui o membro do GT" style="cursor:pointer;" src="/imagens/exclui_p.gif" border="0" onclick="excluiMembro(this.parentNode.parentNode.rowIndex);" /></td>' +
								'<td><input type="hidden" name="pesnome[]" value="'+jQuery('#pesnome').val()+'" />'+jQuery('#pesnome').val()+'</td>' +
								'<td><input type="hidden" name="dpecpf[]" value="'+jQuery('#dpecpf').val()+'" />'+jQuery('#dpecpf').val()+'</td>' +
								'<td><input type="hidden" name="fgtid[]" value="'+jQuery('#fgtid option:selected').val()+'" />'+jQuery('#fgtid option:selected').text()+'</td>' +
								'<td><input type="hidden" name="dpeddd[]" value="'+jQuery('#dpeddd').val()+'" /><input type="hidden" name="dpetelefone[]" value="'+jQuery('#dpetelefone').val()+'" />('+jQuery('#dpeddd').val()+') '+jQuery('#dpetelefone').val()+'</td>' +
								'<td><input type="hidden" name="dpeemail[]" value="'+jQuery('#dpeemail').val()+'" />'+jQuery('#dpeemail').val()+'</td>' +
							'</tr>';
				jQuery('#corpoTabela', window.opener.document).append(linha);*/
				
				//var data = jQuery('#formMembro').serialize();

				jQuery.ajax({
					   type: "post",
					   url: "pdeinterativo2013.php?modulo=principal/primeirospassos&acao=A",
					   async: false,
					   data: "ajaxInsereMembro=1&grtid="+jQuery('#grtid').val()+"&pesid="+jQuery('#pesid').val()+"&pesnome="+jQuery('#pesnome').val()+"&dpecpf="+jQuery('#dpecpf').val()+"&fgtid="+jQuery('#fgtid option:selected').val()+"&dpeddd="+jQuery('#dpeddd').val()+"&dpetelefone="+jQuery('#dpetelefone').val()+"&dpeemail="+jQuery('#dpeemail').val()+"",
					   success: function(msg)
					   {
					   		if( msg != 'erro' )
					   		{
								var tabela = window.opener.document.getElementById("tbMembros");
								
								if( jQuery('#pesid').val() != '' )
								{
									var linha = tabela.rows[jQuery('#index').val()];

									linha.cells[1].innerHTML = jQuery('#pesnome').val();
									linha.cells[2].innerHTML = jQuery('#dpecpf').val();
									linha.cells[3].innerHTML = jQuery('#fgtid option:selected').text();
									linha.cells[4].innerHTML = '('+jQuery('#dpeddd').val()+') '+jQuery('#dpetelefone').val();
									linha.cells[5].innerHTML = jQuery('#dpeemail').val();
								}
								else
								{
									var linha = tabela.insertRow(3);
									
									var cel1 = linha.insertCell(0);
									var cel2 = linha.insertCell(1);
									var cel3 = linha.insertCell(2);
									var cel4 = linha.insertCell(3);
									var cel5 = linha.insertCell(4);
									var cel6 = linha.insertCell(5);

									cel1.style.textAlign = 'center';
									cel1.innerHTML = '<img style="cursor:pointer;" title="Altera as informações do membro do GT" src="/imagens/check_p.gif" border="0" onclick="alteraMembro(this.parentNode.parentNode.rowIndex, '+msg+');" />&nbsp;<img title="Exclui o membro do GT" style="cursor:pointer;" src="/imagens/exclui_p.gif" border="0" onclick="excluiMembro(this.parentNode.parentNode.rowIndex, '+msg+');" />';
									cel2.innerHTML = jQuery('#pesnome').val();
									cel3.innerHTML = jQuery('#dpecpf').val();
									cel4.innerHTML = jQuery('#fgtid option:selected').text();
									cel5.innerHTML = '('+jQuery('#dpeddd').val()+') '+jQuery('#dpetelefone').val();
									cel6.innerHTML = jQuery('#dpeemail').val();
								}
					   		}
					   		else
					   		{
								alert('Ocorreu um erro ao gravar os dados do membro do GT.');
								return;
					   		}
					   }
				});

				if( jQuery('#pesid').val() != '' )
				{
					alert('Membro do GT atualizado com sucesso.');
					self.close();
				}
				else
				{
					//jQuery("#dialog").dialog("open");
					
					alert('Membro do GT incluído com sucesso.');
					jQuery('#pesnome').val('');
					jQuery('#dpecpf').val('');
					jQuery('#fgtid').val('');
					jQuery('#dpeddd').val('');
					jQuery('#dpetelefone').val('');
					jQuery('#dpeemail').val('');
				}
				
				//$('#formMembro').submit();
			});
		});
		
		function carregaUsuario() {
		    divCarregando();
			var usucpf=document.getElementById('dpecpf').value;
			usucpf = usucpf.replace('-','');
			usucpf = usucpf.replace('.','');
			usucpf = usucpf.replace('.','');
			
   			var comp = new dCPF();
			comp.buscarDados(usucpf);
			var arrDados = new Object();
			if(!comp.dados.no_pessoa_rf){
				alert('CPF Inválido');
		  		divCarregado();
				return false;
			}
			document.getElementById('pesnome').value=comp.dados.no_pessoa_rf;
		    divCarregado();
		  }
		</script>
	</head>
	<body>
	
	<div id="dialog" title="Grupo de Trabalho" style="display:none;">
		<center><img border="0" src="../imagens/atencao.png" /><b>Atenção!</b></center>
		<br />
		Membro do GT inserido com sucesso.
		<br />
		Deseja inserir outro membro?
	</div>

		<form id="formMembro" method="post" action="">
		<input type="hidden" id="grtid" name="grtid" value="<?=$grtid?>" />
		<input type="hidden" id="pesid" name="pesid" value="<?=$pesid?>" />
		<input type="hidden" id="index" name="index" value="<?=$index?>" />
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="3" cellPadding="5" align="center" width="100%">
			<tr>
				<td bgcolor="#c4c4c4" align="center" colspan="2">
					<b>Incluir membro do GT</b>
				</td>
			</tr>
			<tr>
				<td bgcolor="" align="center" colspan="2">
					<img border="0" src="/imagens/obrig.gif" /> Indica campo obrigatório
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita" style="width:100px;">Nome</td>
				<td>
					<?=campo_texto('pesnome', 'S', 'N', '', '40', '180', '', 'N', 'left', '', '', 'id="pesnome"', '', null, '' )?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">CPF</td>
				<td>
					<?=campo_texto('dpecpf', 'S', 'S', '', '20', '14', '###.###.###-##', 'N', 'left', '', '', 'id="dpecpf"', '', null, 'carregaUsuario();' )?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">Perfil</td>
				<td>
				<?php
				$sql = "SELECT
							fgtid as codigo,
							fgtdesc as descricao
						FROM
							pdeinterativo2013.funcaogt
						WHERE
							fgtstatus = 'A'
						ORDER BY
							fgtdesc ASC";
				$db->monta_combo("fgtid", $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'fgtid');
				?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">(DDD) Telefone</td>
				<td>
					( <?=campo_texto('dpeddd', 'N', 'S', '', '5', '2', '##', 'N', 'left', '', '', 'id="dpeddd"', '', null, '' )?> )
					<?=campo_texto('dpetelefone', 'S', 'S', '', '15', '9', '####-####', 'N', 'left', '', '', 'id="dpetelefone"', '', null, '' )?>
				</td>
			</tr>
			<tr>
				<td align="right" class="SubTituloDireita">Email</td>
				<td>
					<?=campo_texto('dpeemail', 'S', 'S', '', '40', '100', '', 'N', 'left', '', '', 'id="dpeemail"', '', null, '' )?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita"></td>
				<td align="left">
					<input type="button" value="Salvar" id="btSalvar" />
					<input type="button" value="Fechar" onclick="self.close();" />
				</td>
			</tr>
		</table>
		</form>
	</body>
</html>