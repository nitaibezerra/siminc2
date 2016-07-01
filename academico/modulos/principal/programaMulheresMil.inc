<?php

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$mmiano=2013;

$vbAbertoPreenchimento=false;
$vbAbertoAnalise=false;
$vbAbertoRecurso=false;
$sql = "SELECT mmaid, mmaaberturapreenchimento, mmafechamentopreenchimento, mmaaberturaanalise, mmafechamentoanalise, mmaaberturarecurso, mmafechamentorecurso
		FROM academico.mulheresmiladministracao
		WHERE mmaano = ".$mmiano;
$rsAdmin = $db->pegaLinha($sql);
if($rsAdmin['mmaid']){
	if(($rsAdmin['mmaaberturapreenchimento'] <= date('Y-m-d')) && ($rsAdmin['mmafechamentopreenchimento'] >= date('Y-m-d'))){
		$vbAbertoPreenchimento=true;
	}
	if(($rsAdmin['mmaaberturaanalise'] <= date('Y-m-d')) && ($rsAdmin['mmafechamentoanalise'] >= date('Y-m-d'))){
		$vbAbertoAnalise=true;
	}
	if(($rsAdmin['mmaaberturarecurso'] <= date('Y-m-d')) && ($rsAdmin['mmafechamentorecurso'] >= date('Y-m-d'))){
		$vbAbertoRecurso=true;
	}
}

if($_SESSION["usucpf"] == ''){
	$vbAbertoPreenchimento=true;
	$vbAbertoAnalise=false;
	$vbAbertoRecurso=false;
}

if($_REQUEST['requisicao'] == 'salvaDados'){
	
	if(empty($_REQUEST['mmiid'])){
		
		$sql = "insert into 
					academico.mulheresmil 
					(entid, mmiano, mmidtinclusao, mmistatus, usucpf)
				values
					({$_SESSION['academico']['entid']}, '{$_REQUEST['mmiano']}', '".date('Y-m-d H:i:s')."', 'A', '".$_SESSION["usucpf"]."')
				returning mmiid; ";
		
		$mmiid = $db->pegaUm($sql);
		$db->commit();
				
	}else{
		
		$mmiid = $_REQUEST['mmiid'];
	}
	
	if(is_array($_REQUEST['campus']) && count($_REQUEST['campus'])){
		
		$sql = '';
		foreach($_REQUEST['campus'] as $campus){
									
			$sql .= " insert into academico.mulheresmilcampus 
					 	(mmiid, cmpid, mmcsituacao)
					 values 
					 	({$mmiid}, {$campus}, 'A'); ";
		}
		
		$sql = " delete from academico.mulheresmilcampus where mmiid = {$mmiid}; ".$sql;
		$db->executar($sql);
		$db->commit();
	}
	
	if(!empty($_FILES['arquivo']['name'])){	
		if($vbAbertoPreenchimento){
			$tipo = 'C';
		}
		if($vbAbertoRecurso){
			$tipo = 'R';
		}
		
		$sql = "update academico.mulheresmilanexos set mmxstatus = 'I' where mmiid = ".$mmiid." and mmxtipo = '".$tipo."'";
		$db->executar($sql);
		$db->commit();
		
		$mmxdesc = $_REQUEST['mmxdesc'] ? "'".$_REQUEST['mmxdesc']."'" : 'null';
	
		$campos = array('mmiid'				=> $mmiid,
						'mmxdesc'			=> $mmxdesc,
						'mmxdtinclusao'		=> "'".date('Y-m-d H:i:s')."'",
						'mmxstatus'			=> "'A'",
						'mmxtipo'			=> "'".$tipo."'");
		
		$file = new FilesSimec("mulheresmilanexos", $campos ,"academico");
			
		$arquivoSalvo = $file->setUpload();
	}	
		
	$db->sucesso('principal/programaMulheresMil');
}

if($_REQUEST['arqid']){
	ob_clean();
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'academico.php?modulo=principal/programaMulheresMil&acao=A';</script>";
    exit;
}

require_once APPRAIZ . "includes/cabecalho.inc";

echo '<br/>';
$abacod_tela = 57267;
$db->cria_aba($abacod_tela,$url,$parametros);
if($mmiano==2012){
	academico_monta_cabecalho_sig($_SESSION['academico']['entid'], 'Chamada Pública Mulheres Mil');
}else{
	academico_monta_cabecalho_sig($_SESSION['academico']['entid'], 'Adesão Programa Nacional Mulheres Mil');
}

$entids = array(390623,388731,388732,390645,390450,388754,391995,388865,388734,388735,388853,388733,388736,388761,388753,388755
			,390628,388737,388758,388739,388740,388742,388743,390185,388744,171206,391144,388749,388756,388748,388746,388759,388747,388760
			,388741,388738,388750,388763);

if( !in_array($_SESSION['academico']['entid'], $entids) ) {
	echo "<script>alert('Entidade Não Participante'); history.back(-1);</script>";
	die;
}


$sql = "select * from academico.mulheresmil where entid = ".$_SESSION['academico']['entid']." AND mmiano = ".$mmiano;
$rs = $db->pegaLinha($sql);

//ARQUIVO DA CHAMADA PÚBLICA
if($rs['mmiid']){
	$sql = "select 
				* 
			from 
				academico.mulheresmilanexos ma
			inner join 
				public.arquivo arq on arq.arqid = ma.arqid
			where 
				ma.mmiid = {$rs['mmiid']} 
			and 
				ma.mmxstatus = 'A' 
			and
				ma.mmxtipo = 'C'
			order by 
				ma.mmxid desc 
			limit 1";
	
	$rsAnexo = $db->pegaLinha($sql);
}else{
	$rsAnexo = array();
}

//ARQUIVO DO RECURSO
if($rs['mmiid']){
	$sql = "select 
				* 
			from 
				academico.mulheresmilanexos ma
			inner join 
				public.arquivo arq on arq.arqid = ma.arqid
			where 
				ma.mmiid = {$rs['mmiid']} 
			and 
				ma.mmxstatus = 'A' 
			and
				ma.mmxtipo = 'R'
			order by 
				ma.mmxid desc 
			limit 1";
	
	$rsAnexo2 = $db->pegaLinha($sql);
}else{
	$rsAnexo2 = array();
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
jQuery.noConflict();
function salvarDados()
{	
	<?php if($vbAbertoPreenchimento){ ?>
		selectAllOptions( document.getElementById('campus') );
		campus = jQuery('#campus option:selected');
	
		if(campus[0].value == ''){
			alert('Informe pelo menos um campus!');
			jQuery('#campus').focus();
			return false;
		}
	<?php }?>
	
	arqid = jQuery('[name=arqid]').val();
	arquivo = jQuery('[name=arquivo]').val();
	
	if(!arqid && arquivo == ''){
		alert('O anexo é obrigatório!');
		jQuery('[name=arquivo]').focus();
		return false;
	}
	
	document.formulario.submit();
}
</script>
<form name="formulario" method="post" enctype="multipart/form-data">
	<input type="hidden" name="requisicao" value="salvaDados" />
	<input type="hidden" name="mmiid" value="<?php echo $rs['mmiid'] ? $rs['mmiid'] : '' ?>" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtituloEsquerda">Ano:</td>
			<td>
				<?php echo $mmiano ?>
				<input type="hidden" value="<?php echo $mmiano ?>" name="mmiano" />
			</td>
		</tr>
		<?php
		if($mmiano==2012){
		?>
			<tr>
				<td class="subtituloEsquerda">Chamada Pública:</td>
				<td>
					<a target=\"_blank\" href="arquivos/chamada_publica_2012.pdf">Clique aqui</a>
				</td>
			</tr>
			<tr>
				<td class="subtituloEsquerda">Formulário Para Download:</td>
				<td>
					<a href="arquivos/formulario_adesao_2012.doc">Clique aqui</a>
				</td>
			</tr>
		<?php
		}else{
		?>
			<tr>
				<td class="subtituloEsquerda">Formulário Para Download:</td>
				<td>
					<a href="arquivos/formulario_adesao_2013.doc">Clique aqui</a>
				</td>
			</tr>
		<?php
		}
		?>
		<tr>
			<td class="subtituloEsquerda">Universo preliminar do Censo Demográfico 2010 (IBGE):</td>
			<td>
				<a href="arquivos/censo_demografico_2010.xls">Clique aqui</a>
			</td>
		</tr>
		<tr>
			<td class="subtituloEsquerda">Campus Participantes:</td>
			<td>
				 <?php
				 	if($vbAbertoPreenchimento){
						switch ( $_SESSION['academico']['orgid'] ){
							case '1':
								$funid = ACA_ID_CAMPUS;
							break;
							case '2':
								$funid = ACA_ID_UNED;
							break;
						}
						
						if(isset($rs['mmiid'])){
							
							$sql = "SELECT
									cp.cmpid as codigo, 
									upper(e.entnome) as descricao
								FROM						
									academico.mulheresmilcampus mc
								INNER JOIN 
									academico.campus cp ON cp.cmpid = mc.cmpid
								INNER JOIN 	
									entidade.entidade e ON e.entid = cp.entid
								WHERE 
									mc.mmiid = ".$rs['mmiid'];
							
							$campus = $db->carregar($sql);
						}
					 
						$sql = "SELECT
								cp.cmpid as codigo, 
								upper(e.entnome) as descricao
							FROM						
								entidade.entidade e 
							INNER JOIN
								entidade.funcaoentidade ef ON ef.entid = e.entid
							INNER JOIN
								academico.campus cp ON cp.entid = e.entid
							INNER JOIN
								entidade.funentassoc ea ON ea.fueid = ef.fueid						  
							LEFT JOIN
								entidade.endereco ed ON ed.entid = e.entid
							LEFT JOIN
								territorios.municipio mun ON mun.muncod = ed.muncod
							WHERE
								ea.entid = {$_SESSION['academico']['entid']} AND
								e.entstatus = 'A' AND ef.funid = ".$funid."
							ORDER BY
								e.entnome";
						
						combo_popup( 'campus', $sql, '', '400x600', 0, array(), '', 'S', false, false, null, 500);
					}else{
						echo("Prazo fechado para a escolha dos campus participantes");
					}
				 ?>
			</td>
		</tr>
		<tr>
			<td class="subtituloEsquerda">Anexar:</td>
			<td>
				<?php if($vbAbertoPreenchimento){ ?>
					<input type="file" name="arquivo" />
				<?php
				}else{
					echo("Prazo fechado para anexar o formulário de adesão");
				} ?>
				<?php if($rsAnexo['arqid']): ?>
					<p>
					Arquivo anexado: 
					<a href="academico.php?modulo=principal/programaMulheresMil&acao=A&arqid=<?php echo $rsAnexo['arqid'] ?>">
						<img src="/imagens/clipe1.gif" border="0" align="absmiddle"/>
						<?php echo $rsAnexo['arqnome'].'.'.$rsAnexo['arqextensao']; ?></a>
						<?php
						if(trim($rsAnexo['mmxdesc'])){
							echo '<br/>';
							echo "(".$rsAnexo['mmxdesc'].")";
						}?>
						<?php if($vbAbertoPreenchimento){ ?>
							<input type="hidden" name="arqid" value="<?php echo isset($rsAnexo['arqid']) ? $rsAnexo['arqid'] : '' ?>" />
						<?php }?>
					</p>
				<?php endif; ?>
			</td>
		</tr>
		<?php
		if($rsAnexo['arqid']){ ?>
			<tr>
				<td class="subtituloEsquerda">Anexar Recurso:</td>
				<td>
					<?php if($vbAbertoRecurso){ ?>
						<input type="file" name="arquivo" />
					<?php
					}else{
						echo("Prazo fechado para impetrar recurso");
					} ?>
					<?php if($rsAnexo2['arqid']): ?>
						<p>
						Arquivo anexado: 
						<a href="academico.php?modulo=principal/programaMulheresMil&acao=A&arqid=<?php echo $rsAnexo2['arqid'] ?>">
							<img src="/imagens/clipe1.gif" border="0" align="absmiddle"/>
							<?php echo $rsAnexo2['arqnome'].'.'.$rsAnexo2['arqextensao']; ?></a>
							<?php
							if(trim($rsAnexo2['mmxdesc'])){
								echo '<br/>';
								echo "(".$rsAnexo2['mmxdesc'].")";
							}?>
							<input type="hidden" name="arqid" value="<?php echo isset($rsAnexo2['arqid']) ? $rsAnexo2['arqid'] : '' ?>" />
						</p>
					<?php endif;?>
				</td>
			</tr>
		<?php
		}
		if($vbAbertoPreenchimento || ($rsAnexo['arqid'] && $vbAbertoRecurso)){
		?>
			<tr>
				<td class="subtituloEsquerda">Observações:</td>
				<td>
					<?php echo campo_textarea('mmxdesc', 'N', 'S', 'Obs', 100, 4, 255) ?>
				</td>
			</tr>
			<tr>
				<td class="subtituloEsquerda"></td>
				<td>
					<input type="button" value="Salvar" onclick="salvarDados()" />
				</td>
			</tr>
		<?php
		}?>
	</table>
	<?php
	if($rs['mmiid']){
		
		$sql = "SELECT 
					upper(e.entnome) as descricao,
					CASE mmcsituacao
					WHEN 'A' THEN 'Em andamento'
					WHEN 'V' THEN 'Aprovado'
					WHEN 'R' THEN 'Reprovado'
					END as situacao					
				FROM						
					academico.mulheresmilcampus mc
				INNER JOIN 
					academico.campus cp ON cp.cmpid = mc.cmpid
				INNER JOIN 	
					entidade.entidade e ON e.entid = cp.entid
				WHERE 
					mc.mmiid = ".$rs['mmiid'];
		
		$cabecalho = array('Campus', 'Situação');
		$db->monta_lista($sql, $cabecalho,10,10,'N', '','','');
	}
	?>
</form>