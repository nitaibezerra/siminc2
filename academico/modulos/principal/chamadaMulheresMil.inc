<?php
$mmiano = $_REQUEST['mmiano'];
if ($mmiano == ''){
	$mmiano = '2013';
}
$academico = new academico();

if ($_REQUEST['requisicao'] == 'abrirConteudo') {
	$_SESSION['academico']['orgid'] = $_SESSION['academico']['orgid'] ? $_SESSION['academico']['orgid'] : $_REQUEST["orgid"];
	?>
	<center>
		<fieldset style="background: #fff; width: 95%;">
			<legend align="center">Lista de Campi Participantes</legend>
			<?php
						
			$sql = "SELECT
						
						upper(e.entnome) as nome,
						upper(mun.mundescricao) as municipio, upper(mun.estuf) as uf,
						CASE mmcsituacao
						WHEN 'A' THEN 'Em andamento'
						WHEN 'V' THEN 'Aprovado'
						WHEN 'R' THEN 'Reprovado'
						END as situacao	
					FROM					
						entidade.entidade e
					INNER JOIN
						entidade.funcaoentidade ef ON ef.entid = e.entid
					INNER JOIN
						entidade.funentassoc ea ON ea.fueid = ef.fueid 
					INNER JOIN 
						academico.campus cp ON cp.entid = e.entid 
					INNER JOIN 
						academico.mulheresmilcampus mc ON mc.cmpid = cp.cmpid
					INNER JOIN 
						academico.mulheresmil mm ON mm.mmiid = mc.mmiid AND 
							mm.mmiano = {$_REQUEST["mmiano"]}
					LEFT JOIN
						entidade.endereco ed ON ed.entid = e.entid
					LEFT JOIN
						territorios.municipio mun ON mun.muncod = ed.muncod
					WHERE
						ea.entid = {$_REQUEST["entid"]} 
					AND
						e.entstatus = 'A' 
					AND
						ef.funid = 17
					ORDER BY
						e.entnome";			
			
			$cabecalho = array('Campus', 'Município', 'UF', 'Situação');
			$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '95%');
			?>
		</fieldset>
	</center>	
	<?
	die;
}

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_REQUEST['arqid']){
	ob_clean();
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'academico.php?modulo=principal/chamadaMulheresMil&acao=A';</script>";
    exit;
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

if($mmiano==2012){
	monta_titulo('Chamada Pública Mulheres Mil','');
}else{
	monta_titulo('Adesão Programa Nacional Mulheres Mil','');
}




?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
jQuery.noConflict();

function mudaano()
{
	jQuery("form#formulario").submit();
}

jQuery(document).ready(function(){
	 jQuery('#mmiano').change(function (){
      	mudaano();
     }); 
});


jQuery(function(){
	
});

function abrirConteudo(entid)
{
	if(jQuery('#td'+entid).css('display') == 'none'){
		
		jQuery('#td'+entid).html('Carregando...').show();
		
		jQuery.ajax({
			url: 'academico.php?modulo=principal/chamadaMulheresMil&acao=A',
			type: 'post',
			data: 'requisicao=abrirConteudo&entid='+entid+'&mmiano=<?=$mmiano?>',
			success: function(e){				
				jQuery('#td'+entid).html(e);
				
			}
		});		
		jQuery('#img'+entid).attr('src','../imagens/menos.gif')
		
	}else{
	
		jQuery('#img'+entid).attr('src','../imagens/mais.gif')
		jQuery('#td'+entid).hide();
	}
}

function downloadAnexo(arqid)
{	
	document.location.href = 'academico.php?modulo=principal/chamadaMulheresMil&acao=A&arqid='+arqid;
}
</script>
<!--
<form name="formulario" id="pesquisar" method="POST" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td  bgcolor="#CCCCCC" colspan="2"><b>Argumentos da Pesquisa</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Instituição</td>
			<td>
				<?php
				
					$orgid = $_SESSION['academico']['orgid'];
					switch ( $orgid ){
						case '1':
							$funid = " in ('" . ACA_ID_UNIVERSIDADE . "')";
						break;
						case '2':
							$funid = " in ('" . ACA_ID_ESCOLAS_TECNICAS . "')";
						break;
					}
						
					$sql_unidade = "SELECT
										e.entid as codigo,
										e.entnome as descricao
									FROM
										entidade.entidade e
									INNER JOIN
										entidade.funcaoentidade ef ON ef.entid = e.entid
									WHERE
										e.entstatus = 'A' AND ef.funid {$funid}
									ORDER BY
										e.entnome";
					
					$entid = $_REQUEST['entid'] ? $_REQUEST['entid'] : ''; 
					
					$db->monta_combo("entid", $sql_unidade, "S", "Todas as Instituições", '', '', '', '', 'N','entid');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF</td>
			<td>
				<?php
					
					$sql_uf = "SELECT
								estuf as codigo,
								estdescricao as descricao
							FROM
								territorios.estado";
					
					$estuf = $_REQUEST['estuf'] ? $_REQUEST['estuf'] : '';
					
					$db->monta_combo("estuf", $sql_uf, "S", "Todas", '', '', '', '150', 'N','estuf');
					
				?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input style="cursor:pointer;" type="submit" value="Pesquisar" />
				<input style="cursor:pointer;" type="button" value="Ver Todos" onclick="document.location.href = 'academico.php?modulo=principal/chamadaMulheresMil&acao=A'" />
			</td>
		</tr>
	</table>
</form>
-->
<form name="formulario" id="formulario" method="POST" action="">
<table class="tabela" height="95%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >		
		<tr>
			<td width='1' align='right'>Ano:</td>
			<td>
				<select class="CampoEstilo obrigatorio" name="mmiano" id = "mmiano" >
					<option <?php echo( $mmiano=="2012" ? "selected": ""); ?> value="2012">2012</option>
					<option <?php echo( $mmiano=="2013" ? "selected": ""); ?> value="2013">2013</option>
				</select>
			</td>
		</tr>
</table>
</form>		
			
<?php
$stWhere = '';
if($_REQUEST['estuf']){
	$stWhere .= " AND mun.estuf = '{$_REQUEST['estuf']}' ";
}

if($_REQUEST['entid']){
	$stWhere .= " AND e.entid = '{$_REQUEST['entid']}' ";
}


$conteudo .= "'<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' || e.entid || '\" colspan=\"2\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr";
$perfis = pegaPerfilGeral();
if(in_array(PERFIL_SUPERUSUARIO, $perfis) || in_array(PERFIL_ADMINISTRADOR, $perfis) || in_array(PERFIL_ALTA_GESTAO, $perfis) || in_array(PERFIL_ADM_MULHERES_MIL, $perfis)){
	$linkanexo = "'<a href=\"javascript:void(0)\" onclick=\"downloadAnexo(' || ma.arqid || ');\"><center><img src=\"/imagens/clipe1.gif\" border=\"0\" /></a></center>'";
}else{
	$linkanexo = "'<center><img src=\"/imagens/clipe1.gif\" border=\"0\" /></center>'";
}

$sql = "SELECT
		'<center><img src=\"../imagens/mais.gif\" id=\"img' || e.entid || '\" style=\"padding-right: 5px; cursor: pointer;\" border=\"0\" width=\"9\" height=\"9\" align=\"absmiddle\" vspace=\"3\" onclick=\"abrirConteudo(' || e.entid || ')\"/></center>' as img,
		UPPER(entsig) ||  ' - ' || UPPER(entnome) as nome,
	mun.mundescricao,
	mun.estuf,
	(SELECT CASE WHEN ma.arqid IS NOT NULL THEN " . $linkanexo . " WHEN ma.arqid IS NULL THEN ' - ' END FROM academico.mulheresmilanexos ma WHERE ma.mmiid = mm.mmiid AND mmxstatus = 'A' AND mmxtipo = 'C') AS AnexoChamada,
	(SELECT CASE WHEN ma.arqid IS NOT NULL THEN " . $linkanexo . " WHEN ma.arqid IS NULL THEN ' - ' END FROM academico.mulheresmilanexos ma WHERE ma.mmiid = mm.mmiid AND mmxstatus = 'A' AND mmxtipo = 'R') AS AnexoRecurso,			
	{$conteudo}	
FROM
	entidade.entidade e
INNER JOIN
	entidade.funcaoentidade ef ON ef.entid = e.entid
INNER JOIN
	entidade.endereco ed ON ed.entid = e.entid
INNER JOIN
	territorios.municipio mun ON mun.muncod = ed.muncod
LEFT JOIN
	academico.mulheresmil mm ON mm.entid = e.entid AND 
	mm.mmiano = {$mmiano}
WHERE
	e.entstatus = 'A'

AND
	ef.funid = ". ACA_ID_ESCOLAS_TECNICAS ."
	{$stWhere}
AND
	e.entid in (390623,388731,388732,390645,390450,388754,391995,388865,388734,388735,388853,388733,388736,388761,388753,388755 ,390628,388737,388758,388739,388740,388742,388743,390185,388744,171206,391144,388749,388756,388748,388746,388759,388747,388760 ,388741,388738,388750,388763)
GROUP BY
	e.entid, e.entnome, e.entsig, mun.mundescricao, mun.estuf,mm.mmiid
ORDER BY
	e.entsig, e.entnome";

$cabecalho = array('Ação', 'Instituição', 'Município', 'UF', 'Adesão', 'Recurso');
$db->monta_lista( $sql, $cabecalho, 100, 30, 'N', 'center', '');

?>