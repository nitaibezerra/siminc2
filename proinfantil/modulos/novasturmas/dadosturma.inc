<?php
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include  "_funcoes_novasturmas.php";
$file = new FilesSimec();

//DOWNLOAD ARQUIVO
if($_REQUEST['requisicao'] == 'downloadArquivo'){
	$file->getDownloadArquivo($_REQUEST['arqid']);
	exit();
}

//Recupera Turmas do Município
$sql = "SELECT
				    tre.tirdescricao, 
				    tur.ttuid,
				    CASE WHEN tur.ttuid = 1 THEN 'Exclusivo Creche'
						 WHEN tur.ttuid = 2 THEN 'Exclusivo Pré-escola'
						 WHEN tur.ttuid = 3 THEN 'Creche e Pré-escola (Misto)'
				    END AS tipodependencia,  
				    CASE WHEN tur.turtipoestabelecimento = 1 THEN 'Municipal ou Distrital'
						 WHEN tur.turtipoestabelecimento = 3 THEN 'Conveniado (Comunitário, confessional ou filantrópico)'
				    END AS tipoestabelecimento,  				    
				    tur.turdsc,
				    to_char(tur.turdtinicio,'DD/MM/YYYY') AS turdtinicio,
				    CASE WHEN tur.turcadeducacenso = 't' THEN 'Sim'
						 WHEN tur.turcadeducacenso = 'f' THEN 'Não'
				    END AS educacenso,  				    
				    CASE WHEN tur.turestabelecimentoautorizado = 't' THEN 'Sim'
						 WHEN tur.turestabelecimentoautorizado = 'f' THEN 'Não'
				    END AS estabelecimentoautorizado,  					    
					ent.entcodent || ' - ' || ent.entnome as codinep,
			    	CASE WHEN tur.turtipoconselho = 'E' THEN 'Conselho Estadual de Educação'
						 WHEN tur.turtipoconselho = 'M' THEN 'Conselho Municipal de Educação'
				    END AS tipoconselho,  								
				    tur.turid,
					tur.entcodent,
				    CASE WHEN tur.turenderecocodinep = 't' THEN 'Sim'
						 WHEN tur.turenderecocodinep = 'f' THEN 'Não'
					 END AS enderecocodinep, 					
					tur.turnomeescola,
					tur.arqid,
					tur.turenderecoescola,
					tur.turtipoestabelecimento,
					tur.turlatitude, 
					tur.turlongitude,
					tur.turcepescola,					
					tur.turnumeroato,
					tur.turdatapublicacaoato,
					arq.arqdescricao
		FROM 		proinfantil.turma tur
		INNER JOIN 	proinfantil.tiporede tre ON tre.tirid = tur.tirid
		INNER JOIN  public.arquivo arq ON arq.arqid = tur.arqid
		LEFT JOIN   entidade.entidade ent ON trim(ent.entcodent) = trim(tur.entcodent)
		WHERE muncod = '{$_SESSION['proinfantil']['muncod']}'
		AND turstatus = 'A'
		AND turid = {$_REQUEST['turma']}";

$rs_turma = $db->pegaLinha($sql);
$nome_turma = "Turma: ".$rs_turma['turdsc'];

$docid = criaDocumentoNovasTurmas($_SESSION['proinfantil']['muncod']);
$esdid = pegaEstadoAtualNovasTurmas($docid);
	
$acesso = verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

$arrSituacao = array(WF_NOVASTURMAS_EM_CADASTRAMENTO, WF_NOVASTURMAS_EM_DILIGENCIA);
$perfis = pegaPerfil($_SESSION['usucpf']);

monta_titulo($nome_turma, '');

$_SESSION['imgparams'] = array("filtro" => "1=1", "tabela" => "proinfantil.fotos");
?>
<style>
	.field_foto {text-align:right}
	.field_foto legend{font-weight:bold;}
	.img_add_foto{cursor:pointer}
	.hidden{display:none}
	.img_foto{border:0;margin:5px;cursor:pointer;margin-top:-5px}
	.div_foto{width:110px;height:90px;margin:3px;border:solid 1px black;float:left;text-align:center;background-color:#FFFFFF}
	.fechar{position:relative;margin-left:104px;top:-6px;cursor:pointer}
</style>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script
	type="text/javascript" src="/includes/estouvivo.js"></script>
<link
	rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link
	rel="stylesheet" type="text/css" href="../includes/listagem.css" />
<?php cabecalhoTurma(); ?>

<table border="0" class="tabela" align="center" bgcolor="#f5f5f5"
	cellspacing="1" cellpadding="3">
	<tr>
		<td class="subtituloDireita">Tipo de Rede</td>
		<td><?php echo $rs_turma['tirdescricao']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Tipo de atendimento da nova turma</td>
		<td><?php echo $rs_turma['tipodependencia']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Tipo do estabelecimento</td>
		<td><?php echo $rs_turma['tipoestabelecimento']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita" width="30%">Nome da nova turma</td>
		<td><?php echo $rs_turma['turdsc']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Data início do atendimento as crianças</td>
		<td><?php echo $rs_turma['turdtinicio']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Estabelecimento está cadastrado no
			Educacenso 2011?</td>
		<td><?php echo $rs_turma['educacenso']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Código INEP</td>
		<td><?php echo $rs_turma['codinep']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Esta turma funciona no endereço do código
			INEP?</td>
		<td><?php echo $rs_turma['enderecocodinep']; ?></td>
	</tr>
	<?php if($rs_turma['enderecocodinep'] == 'f'){ ?>
	<tr>
		<td class="subtituloDireita">Nome da escola</td>
		<td><?php echo $rs_turma['turnomeescola']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">CEP</td>
		<td><?php $turcepescola = formata_cep($rs_turma['turcepescola']); ?> <?php echo $turcepescola; ?>
		</td>
	</tr>
	<tr>
		<td class="subtituloDireita">Endereço da escola</td>
		<td><?php echo $rs_turma['turenderecoescola']; ?></td>
	</tr>
	<?php
	$latitude  = explode('.',$rs_turma['turlatitude']);
	$longitude = explode('.',$rs_turma['turlongitude']);
	?>
	<tr>
		<td class="SubTituloDireita">Latitude :</td>
		<td><span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?>
		</span> - <span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?>
		</span> ' <span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?>
		</span> " <span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?>
		</span>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Longitude :</td>
		<td><span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?>
		</span> - <span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?>
		</span> ' <span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?>
		</span> " <span id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?>
		</span>
		</td>
	</tr>
	<?php } ?>
	<tr>
		<td class="subtituloDireita">O estabelecimento tem ato autorizativo do
			respectivo sistema de ensino?</td>
		<td><?php echo $rs_turma['estabelecimentoautorizado']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Emitido pelo</td>
		<td><?php echo $rs_turma['tipoconselho']; ?></td>
	</tr>
	<tr class="campos_ato" style="display: none;">
		<td class="subtituloDireita">Nº do Ato</td>
		<td><?php echo $rs_turma['turnumeroato']; ?></td>
	</tr>
	<tr class="campos_ato" style="display: none;">
		<td class="subtituloDireita">Data de Publicação</td>
		<td><?php echo $rs_turma['turdatapublicacaoato']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Anexo</td>
		<td><a
			href="proinfantil.php?modulo=novasturmas/dadosturma&acao=A&turid=<?php echo $rs_turma['arqid'];?>&arqid=<?php echo $rs_turma['arqid'];?>&requisicao=downloadArquivo">
				<img border="0" src="../imagens/anexo.gif" style="cursor: pointer;" />&nbsp;<?php echo $rs_turma['arqdescricao']; ?>
		</a>
		</td>
	</tr>
	<tr>
		<td colspan="2" height="100" class="quantidade_alunos"><?php 
		$sql = "SELECT titid as codigo, titdescricao as descricao
					FROM proinfantil.tipoturno
					WHERE titstatus = 'A' AND modid = 2
					ORDER BY titdescricao";
			
		$rsTurnos = $db->carregar($sql);
		$rsTurnos = $rsTurnos ? $rsTurnos : array();
		?>
			<table class="tabela" align="center" bgcolor="#f5f5f5"
				cellSpacing="1" cellPadding=3 style="width: 80%;">
				<tr class="subtituloEsquerda">
					<td>Tipo turno</td>
					<?php if($rs_turma['ttuid'] == 1 || $rs_turma['ttuid'] == 3) { ?>
					<td width="180" class="alaquantidade_creche">Qtd. Alunos Creche</td>
					<td width="180" class="alaquantidade_creche">Qtd. Professores
						Creche</td>
						<?php } ?>
						<?php if($rs_turma['ttuid'] == 2 || $rs_turma['ttuid'] == 3) { ?>
					<td width="180" class="alaquantidade_pre">Qtd. Alunos Pré-Escola</td>
					<td width="180" class="alaquantidade_pre">Qtd. Professores
						Pré-Escola</td>
						<?php } ?>
				</tr>
				<?php foreach($rsTurnos as $turnos): ?>
				<?php
				$alaquantidade = '';
				if($rs_turma['turid']){
					$sql = "SELECT alaid ,alaquantidade ,alaquantidadeprofessor
								FROM proinfantil.mdsalunoatendidopbf 
								WHERE turid = {$rs_turma['turid']} AND titid = {$turnos['codigo']} AND timid = 1";

					$rs_pre = $db->pegaLinha($sql);
					$alaquantidade_pre = $rs_pre['alaquantidade'];
					$alaquantidadeprofessor_pre = $rs_pre['alaquantidadeprofessor'];
					$alaid_pre = $rs_pre['alaid'];

					$sql = "SELECT alaid, alaquantidade, alaquantidadeprofessor
								FROM proinfantil.mdsalunoatendidopbf 
								WHERE turid = {$rs_turma['turid']} AND titid = {$turnos['codigo']} AND timid = 3";

					$rs_creche = $db->pegaLinha($sql);
					$alaquantidade_creche = $rs_creche['alaquantidade'];
					$alaquantidadeprofessor_creche = $rs_creche['alaquantidadeprofessor'];
					$alaid_creche = $rs_creche['alaid'];
				} ?>
				<tr>
					<td><?php echo $turnos['descricao']; ?></td>
					<?php if($rs_turma['ttuid'] == 1 || $rs_turma['ttuid'] == 3) { ?>
					<td class="alaquantidade_creche"><?php echo $alaquantidade_creche; ?>
					</td>
					<td class="alaquantidade_creche"><?php echo $alaquantidadeprofessor_creche; ?>
					</td>
					<?php } ?>
					<?php if($rs_turma['ttuid'] == 2 || $rs_turma['ttuid'] == 3) { ?>
					<td class="alaquantidade_pre"><?php echo $alaquantidade_pre; ?></td>
					<td class="alaquantidade_pre"><?php echo $alaquantidadeprofessor_pre; ?>
					</td>
					<?php } ?>
				</tr>
				<?php endforeach; ?>
			</table>
		</td>
	</tr>
</table>

<?php 
monta_titulo('Fotos', $titulo2); 

$sql = "SELECT 			distinct sal.salid,tis.tisdescricao
		FROM			proinfantil.tiposala tis
		INNER JOIN		proinfantil.sala sal ON sal.tisid = tis.tisid
		WHERE			sal.modid = 2
		ORDER BY		tis.tisdescricao";

$arySalas = $db->carregar($sql);
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">				
	<?php if($arySalas){ ?>
		<?php $i=0; ?>
		<?php foreach($arySalas as $sala){ ?>
			<?php echo $i%2 == 0 ? "<tr>" : "" ?>
			<td width="50%" valign="top" id="td_sala_<?php echo $sala['salid']; ?>">
				<fieldset id="field_sala_<?php echo $sala['salid'] ?>" class="field_foto" >
					<legend>Fotos - <?php echo $sala['tisdescricao'] ?></legend>
					<table width="100%">
						<tr>
							<td valign="top">
								<?php 
								$sql = "SELECT 			arq.arqid,	arq.arqnome||'.'||arq.arqextensao,	arq.arqdescricao, arq.arqtamanho, fot.pinid
										FROM 			proinfantil.fotos fot
										LEFT JOIN		public.arquivo arq ON arq.arqid = fot.arqid
										WHERE			fot.salid = {$sala['salid']}
										AND				fot.turid = {$_REQUEST['turma']}
										AND				fot.fotstatus = 'A'";
								$arrFotos = $db->carregar($sql); ?>
								<?php if($arrFotos){ ?>
									<?php foreach($arrFotos as $foto){ ?>
										<?php $pagina = floor($n/16); ?>
										<?php
												$imgend = APPRAIZ.'arquivos/'.(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]).'/'. floor($foto['arqid']/1000) .'/'.$foto['arqid'];
												if(is_file($imgend)){
													$img_max_dimX = 100;
													$img_max_dimY = 85;
													
													$imginfo = getimagesize($imgend);
													
													$width = $imginfo[0];
													$height = $imginfo[1];
												
													if (($width >$img_max_dimX) or ($height>$img_max_dimY)){
														if ($width > $height){
														  	$w = $width * 0.9;
															  while ($w > $img_max_dimX){
																  $w = $w * 0.9;
															  }
															  $w = round($w);
															  $h = ($w * $height)/$width;
														  }else{
															  $h = $height * 0.9;
															  while ($h > $img_max_dimY){
																  $h = $h * 0.9;
															  }
															  $h = round($h);
															  $w = ($h * $width)/$height;
														  }
													}else{
														  $w = $width;
														  $h = $height;
													}
													
													$tamanho = " width=\"$w\" height=\"$h\" ";
												}else{
													$tamanho = "";
												}
										?>
										<div id="div_foto_<?php echo $foto['arqid'] ?>" class="div_foto">
											<?php if($acesso['cadastro'] == 'S'){ ?>
													<?if( (in_array( PERFIL_ADMINISTRADOR, $perfis ) || in_array( PERFIL_SUPER_USUARIO, $perfis ) || in_array( EQUIPE_MUNICIPAL, $perfis )) && in_array($esdid, $arrSituacao)  ){ ?>
														<img src="../imagens/fechar.jpeg" title="Remover Foto" class="fechar" onclick="removerFotoSala('<?php echo $foto['arqid'] ?>')" />
													<?php } else { ?>
														<img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar" />
													<?} ?>
											<?php } else { ?>
													<img src="../imagens/fechar_01.jpeg" title="Remover Foto" class="fechar" />
												<?} ?>
											<img onClick="abrirGaleria('<?php echo $foto['arqid'] ?>','<?php echo $pagina ?>','<?php echo $sala['salid'] ?>','<?php echo $foto['pinid'] ?>', '<?php echo $_REQUEST['turma']; ?>')" <?php echo $tamanho ?> onmouseover="return escape('<b>Descrição:</b> <?php echo $foto['arqdescricao'] ?><br /><b>Tamanho(Kb):</b> <?php echo $foto['arqtamanho'] ? round($foto['arqtamanho']/1024,2) : "N/A" ?>');" class="img_foto" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $foto['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=<?=(($_REQUEST["_sisarquivo"])?$_REQUEST["_sisarquivo"]:$_SESSION["sisarquivo"]) ?>" />
										</div>
									<?php } ?>
								<?php } ?>
							</td>
						</tr>
					</table>
				</fieldset>
			</td>
			<?php echo $i%2 == 1 ? "</tr>" : "" ?>
			<?php $i++ ?>
		<?php } ?>
	<?php } ?>
</table>
<script type="text/javascript">
function abrirGaleria(arqid,pagina,salid,pinid, turid){	
	window.open("../slideshow/slideshow/index.php?getFiltro=1&pagina=" + pagina + "&arqid=" + arqid + "&salid=" + salid + "&turid="+turid,"imagem","width=850,height=600,resizable=yes");
}
</script>
<br>