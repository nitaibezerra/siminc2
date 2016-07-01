<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Connection" content="Keep-Alive">
		<meta http-equiv="Expires" content="-1">
		<title>Selecione a(s) Procedência(s)</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script type="text/javascript">

		window.focus();

		$(document).ready(function()
		{
			/*$('.seleciona_todos').click(function()
			{
				var checked;

				checked = ( $(this).attr('checked') ) ? true : false;
				$('.seleciona_todos').each(function(){ $(this).attr('checked', checked); });

				$('[name=proid[]]').each(function()
				{
					if( checked )
					{
						$(this).attr('checked', true);
					}
					else
					{
						$(this).attr('checked', false);
					}

					inclueProcedencia( $(this).attr('id') );
				});
			});*/

			// se algum precisar vir marcado...		
			$('#procedencia option', window.opener.document).each(function()
			{
				if( $(this).val() != '' )
				{
					var id = $(this).val();
					var classe = $('#'+id).parent().parent().attr('class');
					
					if( $('.'+classe).css('display') == 'none' )
					{
						var unpid = classe.substr(3);
						exibeProcedencias(unpid);
					}
					
					$('#'+id).attr('checked', true);
				}
			});
		});

		function inclueProcedencia(proid)
		{
			if( $('#'+proid).attr('checked') )
			{
				$('#procedencia option', window.opener.document).each(function() { if( $(this).val() == '' ) $(this).remove(); } );
				$('#procedencia', window.opener.document).append( $("<option></option>").attr("value", $('#'+proid).val() ).text( $('#desc_proc_'+proid).html() ) );
			}
			else
			{
				$('#procedencia option', window.opener.document).each(function()
				{
					if( $(this).val() == $('#'+proid).val() )
					{
						$(this).remove();
					}
				});

				if( $('#procedencia option', window.opener.document).length == 0 )
				{
					$('#procedencia', window.opener.document).append( $("<option></option>").attr("value", "" ).text( "Duplo clique para selecionar da lista" ) );
				}
			}
		}

		function exibeProcedencias(unpid)
		{
			if( $('#img_'+unpid).attr('src') == '../imagens/mais.gif' )
			{
				$('#img_'+unpid).attr('src', '../imagens/menos.gif');
				$('.tr_'+unpid).show();
			}
			else
			{
				$('#img_'+unpid).attr('src', '../imagens/mais.gif');
				$('.tr_'+unpid).hide();
			}
		}
		
		</script>
	</head>
	<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0" marginheight="0" bgcolor="#ffffff">
		<form name="combo_popup">
			<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<tr bgcolor="#cdcdcd">
					<td width="1" style="text-align: center;">
						<!--<input type="checkbox" class="seleciona_todos" type="checkbox" value="" />-->
					</td>
					<td colspan="2" valign="top">
						<strong>Selecione a(s) Procedência(s)</strong>
					</td>
				</tr>
				<?php 
				$sql = "SELECT
							unpid as codigo,
							unpdsc as descricao
						FROM
							profeinep.unidadeprocedencia
						WHERE
							undpstatus = 'A'
						ORDER BY
							2";
				$unprocedencia = $db->carregar($sql);
				
				if( $unprocedencia ):
					
					foreach($unprocedencia as $u):
					?>
					<tr bgcolor="#cdcdcd">
						<td width="1" align="center" height="30px">
							<img border="0" style="cursor:pointer;" id="img_<?=$u['codigo']?>" src="../imagens/mais.gif" onclick="exibeProcedencias(<?=$u['codigo']?>);" />
						</td>
						<td colspan="2">
							<?=$u['descricao']?>
						</td>
					</tr>
					<?php
						$sql = "SELECT
									proid AS codigo,
									prodsc AS descricao
								FROM 
									profeinep.procedencia
								WHERE
									unpid = {$u['codigo']}
								ORDER BY
									2";
						$procedencia = $db->carregar($sql);
						
						if( $procedencia ):
							$cont = 0;
							foreach($procedencia as $p):
								$cont++;
								$cor = ($cont%2) ? '#f4f4f4' : '#e0e0e0';
						?>
						<tr bgcolor="<?=$cor?>" class="tr_<?=$u['codigo']?>" style="display:none;">
							<td width="1">
								<input name="proid[]" id="<?=$p['codigo']?>" type="checkbox" value="<?=$p['codigo']?>" onclick="inclueProcedencia(<?=$p['codigo']?>);" />
							</td>
							<td colspan="2" id="desc_proc_<?=$p['codigo']?>"><?=$p['descricao']?></td>
						</tr>
						<?php
							endforeach;
						else: 
						?>
						<tr bgcolor="#f4f4f4">
							<td colspan="3" align="center" style="color:red;">
								Não existem registros.
							</td>
						</tr>
						<?php endif; ?>
					<?php endforeach; ?>
				<?php else: ?>
				<tr bgcolor="#f4f4f4">
					<td colspan="3" align="center" style="color:red;">
						Não existem registros.
					</td>
				</tr>
				<?php endif; ?>
				<tr bgcolor="#cdcdcd">
					<td width="1" style="text-align: center;">
						<!--<input type="checkbox" class="seleciona_todos" type="checkbox" value="" />-->
					</td>
					<td colspan="2" valign="top" class="cabecalho">
						<strong>Selecione a(s) Procedência(s)</strong>
						<input type="button" value="OK" onclick="self.close();">
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>