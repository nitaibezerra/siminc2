<?php 

if( !$_SESSION['pdeinterativo_vars']['pdeid'] ){
	echo "<script>alert('Erro de sessão.');window.close();</script>";
	die();
}

include_once "_funcoes_formacao.php";

function salvaCurso( $dados ){
	
	global $db;
	
	if( $dados['curid'] != '' ){
		$testa_dem = $db->pegaUm('SELECT true FROM catalogocurso.curso WHERE curpademsocial IS TRUE AND curid = '.$dados['curid']);
		if( $testa_dem ){
			$alertDem = "alert('- O curso selecionado atende também à \"demanda social\". Na próxima tela, \"2.3 Demanda Social\", será possível indicar essa demanda com base em uma lista de público-alvo específico.\\n\\nDados salvos com sucesso.');";
		}else{
			$alertDem = "alert('Dados salvos com sucesso.');";
		}
		
		$sql = "UPDATE pdeinterativo.planoformacaodocente SET
					curid = ".$dados['curid'].",
					pcfid = ".$dados['pcfid'][$dados['curid']].",
					modid = ".$dados['modid'][$dados['curid']].",
					pfdemail = '".$dados['pfdemail']."',
					pfdtel = '".$dados['pfdtel']."',
					pfdcel = '".$dados['pfdcel']."',
					pfdhabilitado = null,
					pfdnhainteresse = null,
					pfdnhacurso = null
				WHERE
					pfdid = ".$dados['pfdid'];
		
		$db->executar($sql);
		$db->commit();
		echo "<script>
				$alertDem
				window.opener.location.reload();
				window.close();
			  </script>";
	}
}

function seminteresse( $dados ){
	
	global $db;
	
	$sql = "UPDATE pdeinterativo.planoformacaodocente SET
				pfdnhainteresse = true,
				curid = null,
				pcfid = null,
				modid = null,
				pfdprioridade = null,
				pfdhabilitado = null,
				pfdnhacurso = null
			WHERE
				pfdid = ".$dados['pfdid'];
	
	$db->executar($sql);
	$db->commit();
	echo "<script>
			alert('Dados salvos com sucesso.');
			window.opener.location.reload();
			window.close();
		  </script>";
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
	die();
}


?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>		
		<script type="text/javascript" src="/includes/entidadesn.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		<script type="text/javascript" src="../../includes/remedial.js"></script>
		<script type="text/javascript" src="../../includes/superTitle.js"></script>
		<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
		<script>

			function bloqueado( obj, val ){

				obj.attr('disabled',val);				
				
			}

			function imprimirCurso( curid ){
				if( parseInt(curid) > 0 ){
					return window.open('pdeinterativo.php?modulo=principal/popUpImpressaoCursoFormacao&acao=A&curid='+curid, 
									   'modelo', 
									   "height=600,width=950,scrollbars=yes,top=50,left=200" );
				}
			}
			
			$(document).ready(function() {

				$('#salvar').click(function(){

					bloqueado( $(this), true );

					if( $('[name="pfdemail"]').val() != '' ){
						if( !validaEmail( $('[name="pfdemail"]').val() ) ){
							jQuery('[name="pfdemail"]').focus();
							bloqueado( $(this), false );
							return false;
						}
					}
					
					if( $('[name="semInteresse"]').attr('checked') == true ){
						if( confirm('Não há nenhum curso de interesse? \n Se não, clique em \'OK\' para prosseguir.') ){
							$('#req').val('seminteresse');
							$('#frmCursos').submit();
							return false;
						}else{
							bloqueado( $(this), false );
							return false;
						}
					}

					var curid = $('input[name=curid]:checked').val();
					
					if( $('#pcfid_'+curid).val() == '' ){
						bloqueado( $(this), false );
						alert('Escolha um Período.');
						return false;
					}

					if( $('#modid_'+curid).val() == '' ){
						bloqueado( $(this), false );
						alert('Escolha uma Modalidade de Ensino.');
						return false;
					}

					if( confirm('Cada profissional da escola só pode ser indicado para participar de um curso, mesmo que atue em outras escolas. Confirma esta indicação?') ){
						$('#req').val('salvaCurso');
						$('#frmCursos').submit();
					}
					bloqueado( $(this), false );
				});

				$('#semcurso').click(function(){
					bloqueado( $(this), true );
					if( confirm('Não há nenhum curso de interesse? \n Se não, clique em \'OK\' para prosseguir.') ){
						$('#req').val('seminteresse');
						$('#frmCursos').submit();
					}
					bloqueado( $(this), false );
				});

				$('[name="semInteresse"]').click(function(){
					if( $(this).attr('checked') == true ){
						$('[type="radio"]').attr('disabled',true);
						$('.cursos').hide();
					}else{
						$('[type="radio"]').attr('disabled',false);
						$('.cursos').show();
					}
				});

				if( $('[name="semInteresse"]').attr('checked') == true ){
					$('[type="radio"]').attr('disabled',true);
					$('.cursos').hide();
				}else{
					$('[type="radio"]').attr('disabled',false);
					$('.cursos').show();
				}

			});
		</script>
	</head>
	<body>
		<form method="post" name="frmCursos" id="frmCursos">
			<input type="hidden" name="req" id="req" value=""/>
			<input type="hidden" name="pfdid" id="pfdid" value="<?=$_REQUEST['pfdid'] ?>"/>
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="10" cellPadding="10" align="center">
				<?php 
				
				$verifica = verificaCurso( $_REQUEST['pfdid'] );
				
				if( is_array($verifica) ){
				?>
				<tr>
					<td bgcolor="#c4c4c4" align="center" style="color:red;">
						<b>Docente possui curso cadastrado.</b>
					</td>
				</tr>
				<tr>
					<td>
						Para este professor já foi solicitado o curso '<?=$verifica['curso'] ?>' pela Escola '<?=$verifica['escola'] ?>'. Caso haja interesse em substituir este curso, entre em contato com a escola e solicite o cancelamento da solicitação.
						<br><br>
						<b>Escola:</b> <?=$verifica['escola'] ?>
						<br>
						<b>Município:</b> <?=$verifica['municipio'] ?>
						<br>
						<b>Telefone:</b> <?=$verifica['tel'] ?>
						<br>
						<b>E-mail:</b> <?=$verifica['email'] ?>
						<br><br>
						<b>Informações Complementares:</b><br>
						Cada professor poderá ser indicado somente para um curso, mesmo que lecione em mais de uma escola. Assim, caso ocorra de um professor já ter sido selecionado para um curso por uma outra escola e a escola avalie que o mesmo devería fazer outro curso, será necessário solicitar à primeira escola que cancele esta solicitação. Para isso, a primeira escola deverá marcar a opção 'não há curso de interesse', permitindo que o professor seja novamente disponibilizado para seleção de curso por outro estabelecimento de ensino.
					</td>
				</tr>
				<?php 
				die();
				}
				
				?>
				<tr>
					<td bgcolor="#c4c4c4" align="center">
						<b>Lista de Cursos</b>
					</td>
				</tr>
				<tr>
					<td>
						<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width:100%">
							<tr>
								<td class="subtituloDireita" style="text-align:center" width="60px"><b>Selecione</b></td>
								<td class="subtituloDireita" style="text-align:center" width="100px"><b>Área Temática</b></td>
								<td class="subtituloDireita" style="text-align:center" ><b>Nome do Curso</b></td>
								<td class="subtituloDireita" style="text-align:center" width="100px"><b>Período</b></td>
								<td class="subtituloDireita" style="text-align:center" width="80px"><b>Etapa <br>de Ensino que<br> se destina</b></td>
								<td class="subtituloDireita" style="text-align:center" width="100px"><b>Nivel<br>do Curso</b></td>
								<td class="subtituloDireita" style="text-align:center" width="100px"><b>Modalidade<br> de Ensino</b></td>
								<td class="subtituloDireita" style="text-align:center" width="70px"><b>Carga Horária<br>Total do Curso<br>Min/Máx</b></td>
								<td class="subtituloDireita" style="text-align:center" width="70px"><b>Carga Horária <br>Presencial Exigida<br>Min/Máx(%)</b></td>
							</tr>
							<?php 
							
							$cursos = carregaCursos();
							$cursoSelecionado = carregaCurso( $_REQUEST['pfdid'] );
							
							if( is_array($cursos) ){
								foreach($cursos as $curso){
							?>
							<tr class="cursos">
								<td style="text-align:center" >
									<input type="radio" name="curid" value="<?=$curso['curid'] ?>" <?=$curso['curid'] == $cursoSelecionado['curid'] ? 'checked' : '' ?>/>
								</td>
								<td style="text-align:left" ><?=$curso['atedesc'] ?></td>
								<td style="text-align:rigth" ><a style="cursor:pointer" onclick="imprimirCurso( <?=$curso['curid'] ?> )">(<?=$curso['curid'] ?>)<?=$curso['curdesc'] ?></a></td>
								<td style="text-align:center" >
									<?php 
										$sql = "SELECT DISTINCT
													pcfid as codigo,
													pcfdesc as descricao,
													pcfano
												FROM
													pdeinterativo.periodocursoformacao pcf
												INNER JOIN catalogocurso.curso cur ON (CASE WHEN cur.curfim IS NOT NULL 
																					  		THEN (pcf.pcfano::numeric  BETWEEN to_char(cur.curinicio,'YYYY')::numeric AND to_char(cur.curfim,'YYYY')::numeric)
																					  		ELSE
																					  			(to_char(cur.curinicio,'YYYY')::numeric = pcf.pcfano::numeric 
																								OR to_char(cur.curinicio,'YYYY')::numeric+1 = pcf.pcfano::numeric 
																								OR to_char(cur.curinicio,'YYYY')::numeric+2 = pcf.pcfano::numeric 
																								OR to_char(cur.curinicio,'YYYY')::numeric+3 = pcf.pcfano::numeric )
																					   END ) 
												WHERE
													pcfstatus = 'A'
													AND cur.curid = ".$curso['curid']."
												ORDER BY 
													3,2";
//										ver($sql);
										$db->monta_combo('pcfid['.$curso['curid'].']', $sql, 'S', 'Selecione...', '', '', 'Período', '', 'N', 'pcfid_'.$curso['curid'], '', ($curso['curid'] == $cursoSelecionado['curid'] ? $cursoSelecionado['pcfid'] : '') ); 
									?>
								</td>
								<td style="text-align:center" >
									<img border="0" style="cursor:pointer" align="top" src="../imagens/info.gif" 
										 onmouseover="SuperTitleAjax('pdeinterativo.php?modulo=principal/popUpCursoFormacao&acao=A&req=toolEtapaCurso&curid=<?=$curso['curid'] ?>')" 
									     onmouseout="SuperTitleOff( this )"></td>
								<td style="text-align:left" ><?=$curso['ncudesc'] ?></td>
								<td style="text-align:center" >
									<?php 
										$sql = "SELECT
													mo.modid as codigo,
													mo.moddesc as descricao
												FROM
													catalogocurso.modalidadecurso_curso mc
												INNER JOIN catalogocurso.modalidadecurso mo ON mo.modid = mc.modid
												WHERE
													curid = ".$curso['curid'];
										$db->monta_combo('modid['.$curso['curid'].']', $sql, 'S', 'Selecione...', '', '', 'Modalidade do Curso', '', 'N', 'modid_'.$curso['curid'], '', ($curso['curid'] == $cursoSelecionado['curid'] ? $cursoSelecionado['modid'] : '')); 
									?>
								</td>
								<td style="text-align:center" ><?=$curso['curch'] ?></td>
								<td style="text-align:center" ><?=$curso['curpercpre'] ?></td>
							</tr>
							<?php 
								}
							}else{
								
								$sql = "UPDATE pdeinterativo.planoformacaodocente SET
											curid = null,
											pcfid = null,
											modid = null,
											pfdprioridade = null,
											pfdhabilitado = null,
											pfdnhainteresse = null,
											pfdnhacurso = true
										WHERE
											pfdid = ".$_REQUEST['pfdid'] ;
								$db->executar($sql);
								$db->commit();
							?>
							<tr>
								<td colspan="9" align="center">
									Não existem cursos a serem ofertados.
								</td>
							</tr>
							<?php 								
							}
							?>
							<tr>
								<td align="center">
									<input type="checkbox" name="semInteresse" <?=$cursoSelecionado['pfdnhainteresse'] == 't' ? "checked" : "" ?> value="S"/>
								</td>
								<td colspan="8">Não há Curso de Interesse</td>
							</tr>
						</table>
					</td>
				</tr>
				<?php 
				if( is_array($cursos) ){
					
				
				?>
				<tr>
					<td>
						<u><b>Dados adicionais do docente:</b></u>
					</td>
				</tr>
				<tr>
					<td>
						<b>Nome: </b><?=$cursoSelecionado['no_docente'] ?>
						<br><br>
						<b>Email deste docente:</b>
						<?php echo campo_texto("pfdemail","N","S","Email","50","100","","","","","","","",$cursoSelecionado['pfdemail'])?>
						<br><br>
						<b>Telefone deste docente:</b>
						<?php echo campo_texto("pfdtel","N","S","Telefone","12","12","##-####-####","","","","","","",$cursoSelecionado['pfdtel'])?>
						<br><br>
						<b>Celular deste docente:</b>
						<?php echo campo_texto("pfdcel","N","S","Celular","12","12","##-####-####","","","","","","",$cursoSelecionado['pfdcel'])?>
					</td>
				</tr>
				<tr>
					<td align="center">
						<input type="button" value="Salvar" id="salvar" />
						<input type="button" value="Sair" onclick="window.close();" />
					</td>
				</tr>
				<?php 
				}else{
				?>
				<tr>
					<td align="center">
						<input type="button" value="OK" id="sair"/>
						<input type="button" value="Sair" onclick="window.close();" />
					</td>
				</tr>
				<?php 
				}
				?>
			</table>
		</form>
	</body>
</html>