<?php 

include_once "_funcoes_formacao.php";

?>
<html>
	<head>
		<title>SIMEC - Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
		<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	</head>
	<body>
		<form method="post" name="frmCursos" id="frmCursos">
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="10" cellPadding="10" align="center">
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
							
							foreach($cursos as $curso){
							?>
							<tr>
								<td style="text-align:center" ><input type="radio" name="curid[]" value="<?=$curso['curid'] ?>"/></td>
								<td style="text-align:center" ><?=$curso['atedesc'] ?></td>
								<td style="text-align:rigth" ><?=$curso['curdesc'] ?></td>
								<td style="text-align:center" >
									<?php 
										$sql = "SELECT
													pcfid as codigo,
													pcfdesc as descricao
												FROM
													pdeinterativo.periodocursoformacao
												WHERE
													pcfstatus = 'A'";
										$db->monta_combo('pcfid['.$curso['curid'].']', $sql, 'S', 'Selecione...', '', '', 'Período', '', 'N', 'pcfid['.$curso['curid'].']', '', $curso['pcfid']); 
									?>
								</td>
								<td style="text-align:center" ><img border="0" align="top" src="../imagens/consultar.gif" onclick="mostraEtapaEnsino(<?=$docente['curid'] ?>)"></td>
								<td style="text-align:center" ><?=$curso['ncudesc'] ?></td>
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
										$db->monta_combo('modid['.$curso['curid'].']', $sql, 'S', 'Selecione...', '', '', 'Modalidade do Curso', '', 'N', 'modid['.$curso['curid'].']', '', $curso['modid']); 
									?>
								</td>
								<td style="text-align:center" ><?=$curso['curch'] ?></td>
								<td style="text-align:center" ><?=$curso['curpercpre'] ?></td>
							</tr>
							<?php 
								}
							?>
						</table>
					</td>
				</tr>
				<tr>
					<td align="center">
						<input type="button" value="Salvar" onclick="salvarCurso();" />
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>