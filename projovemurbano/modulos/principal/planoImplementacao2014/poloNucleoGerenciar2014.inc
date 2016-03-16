<?
include_once APPRAIZ."includes/classes/EnderecoCEP.class.inc";

$pmupossuipolo = $db->pegaUm("SELECT pmupossuipolo FROM projovemurbano.polomunicipio pmu 
								WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND tprid = {$_SESSION['projovemurbano']['tprid']} AND pmustatus='A'"); 
//$sql = "SELECT * FROM projovemurbano.polomunicipio plm 
//				INNER JOIN projovemurbano.polo pol ON pol.pmuid = plm.pmuid 
//				WHERE plm.pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND plm.tprid = {$_SESSION['projovemurbano']['tprid']} AND pol.polstatus='A'";
//$habilita = 'S';
//$perfil = pegaPerfilGeral();
//if(in_array(PFL_CONSULTA, $perfil)){
//	$habilita = 'N';
//}

$habilita = 'N';
$perfil = pegaPerfilGeral();
if(in_array(PFL_CONSULTA, $perfil) || in_array(PFL_COORDENADOR_ESTADUAL, $perfil) || in_array(PFL_SECRETARIO_ESTADUAL, $perfil)||in_array(PFL_SUPER_USUARIO, $perfil)|| in_array(PFL_COORDENADOR_MUNICIPAL, $perfil)||in_array(PFL_SECRETARIO_MUNICIPAL, $perfil)|| in_array(PFL_EQUIPE_MEC, $perfil)||in_array(PFL_ADMINISTRADOR, $perfil)){
	$habilita = 'S';
}

if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) {
	$habilitado = "disabled";
	$habilButton = "N";
}
//tprid = {$_SESSION['projovemurbano']['tprid']}
//$sql = "SELECT * FROM territorios.municipio LIMIT 10";
//$sql = "SELECT * FROM projovemurbano.polomunicipio LIMIT 10";
//
//
//$result = $db->pegaLinha($sql);
//ver($result,d);
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
function processaRetornoCEP(retornoCEP) {
	  var dadosEndereco = retornoCEP.split('||');
	//   alert(dadosEndereco);
	  jQuery('#caelogradouro').val(dadosEndereco[0]);
	  jQuery('#caebairro').val(dadosEndereco[1]);
	  jQuery('#endestuf').val(dadosEndereco[3]).change();
	  jQuery('#endmuncod').val(dadosEndereco[4]);
	  jQuery('#caelogradouro_label').html(dadosEndereco[0]);
	  jQuery('#caebairro_label').html(dadosEndereco[1]);
	  jQuery('#endestuf_label').html(dadosEndereco[3]).change();
	  jQuery('#td_endmuncod').html(dadosEndereco[2]);
	  jQuery('#caenumero').val('');
	  jQuery('#caecomp').val('');
	}
function gravarPoloNucleo() {

	<? if($pmupossuipolo=="t") : ?>

	jQuery('#polcep').val(mascaraglobal('#####-###',jQuery('#polcep').val()));

	if(document.getElementById('polcep').value.length!=9) {
		alert('Formato do CEP incorreto. Formato correto: #####-###');
		return false;
	}
	
	if(jQuery('#polcep').val()=='') {
		alert('Digite o CEP');
		return false;
	}
	
	if(jQuery('#polendereco').val()=='') {
		alert('Digite o endereço');
		return false;
	}
	
	if(jQuery('#polbairro').val()=='') {
		alert('Digite o bairro');
		return false;
	}
	
	jQuery('#polnumero').val(mascaraglobal('#######',jQuery('#polnumero').val()));
	
	if(jQuery('#polnumero').val()=='') {
		alert('Digite o número');
		return false;
	}
	
	<? endif; ?>
	
	
	<? if($_SESSION['projovemurbano']['estuf']) : ?>
	selectAllOptions( document.getElementById( 'muncod' ) );
	var erro = false;
	jQuery('#muncod').each(function(){
		if( jQuery(this).val() == '' ){
			alert('Selecione um municipio');
			erro = true;
			return false;
		}
	});
	if( erro ){
		return false;
	}
	<? else : ?>
	if(document.getElementById( 'muncod' ).value=='') {
		alert('Selecione um municipio');
		return false;
	}
	<? endif; ?>
	jQuery('#form').submit();
}

function carregarNucleos(mnuid) {
	jQuery.ajax({
   		type: "POST",
   		url: "projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A",
   		data: "requisicao=carregarNucleos&aba=<?=$_REQUEST['aba'] ?>&mnuid="+mnuid,
   		async: false,
   		success: function(msg){document.getElementById('div_mun_'+mnuid).innerHTML = msg;}
 		});
}

// function getEnderecoPeloCEP(cep) {
// 		jQuery.ajax({
// 	   		type: "POST",
// 	   		url: "projovemurbano.php?modulo=principal/monitoramento2013.inc",
// 	   		data: "requisicao=pegarenderecoPorCEP&endcep="+cep,
// 	   		async: false,
// 	   		success: function(dados){
// 	   				var enderecos = dados.split("||");
// 	   				jQuery('#polendereco').val(enderecos[0]);
// 	   				jQuery('#polbairro').val(enderecos[1]);
// 	   			}
// 	 		});

// }

function gerenciarNucleos(munid) {
	window.open('projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&requisicao=gerenciarNucleos&munid='+munid,'Núcleos','scrollbars=yes,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function editarPoloMunicipio(munid,nucid) {
	window.open('projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&requisicao=gerenciarNucleos&munid='+munid+'&nucid='+nucid,'Núcleos','scrollbars=yes,height=400,width=600,status=no,toolbar=no,menubar=no,location=no');
}

function detalharEndereco(entid) {
    window.open('projovemurbano.php?modulo=principal/indexPoloNucleo2014&acao=A&requisicao=enderecoEscola&entid='+entid, 'Detalhamento do Endereço', 'scrollbars=yes,height=250,width=600,status=no,toolbar=no,menubar=no,location=no')
}
</script>
<?

if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) {
	$habilitado = "disabled";
}


if($_REQUEST['polid']) {
	
	$sql = "SELECT * FROM projovemurbano.polo WHERE polid='".$_REQUEST['polid']."'";
	$polo = $db->pegaLinha($sql);
	
	$requisicao = "atualizarPolo";
	
	extract($polo);
	
	if($_SESSION['projovemurbano']['estuf']) {
		
		$estuf = $_SESSION['projovemurbano']['estuf'];
		
		$muncod = $db->carregar("SELECT mun.muncod as codigo, mun.mundescricao as descricao 
								 FROM projovemurbano.municipio mu 
								 INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mu.munid 
								 INNER JOIN projovemurbano.polo pol ON amp.polid = pol.polid 
								 INNER JOIN territorios.municipio mun ON mun.muncod = mu.muncod
								 WHERE pol.polid='".$_REQUEST['polid']."'");
	}

	if($_SESSION['projovemurbano']['muncod']) {
		$municipio = $db->pegaLinha("SELECT * FROM territorios.municipio WHERE muncod='".$_SESSION['projovemurbano']['muncod']."'");
		extract($municipio);
	}
	
//	ver($municipio);
	
	
} elseif($_REQUEST['munid']) {
	
	if($pmupossuipolo=="t") {
		$polo = "POLO ".$db->pegaUm("SELECT polid FROM projovemurbano.associamucipiopolo WHERE munid='".$_REQUEST['munid']."'");	
	}
	
	$municipio = $db->pegaLinha("SELECT * FROM projovemurbano.municipio mu
								 INNER JOIN territorios.municipio mun ON mun.muncod = mu.muncod  
								 WHERE mu.munid='".$_REQUEST['munid']."'");
	
	$requisicao = "atualizarMunicipio";
	
	extract($municipio);
	
} else {
	
	if($pmupossuipolo=="t") {
		$requisicao = "inserirPolo";	
	} else {
		$requisicao = "inserirMunicipio";
	}
	
	if($_SESSION['projovemurbano']['muncod']) {
		
		$sql = "SELECT mun.muncod, mun.mundescricao, mun.estuf 
				FROM projovemurbano.projovemurbano prj 
				INNER JOIN territorios.municipio mun ON mun.muncod = prj.muncod 
				WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
		
		$projovemurbano = $db->pegaLinha($sql);
		
		extract($projovemurbano);
	}

	if($_SESSION['projovemurbano']['estuf']) {
		
		$sql = "SELECT est.estuf FROM projovemurbano.projovemurbano prj 
				INNER JOIN territorios.estado est ON est.estuf = prj.estuf 
				WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
		
		$projovemurbano = $db->pegaLinha($sql);
		
		extract($projovemurbano);
	}
	

}
//ver($requisicao);
?>
<script language="javascript" type="text/javascript" src="./js/projovemurbano.js"></script>
<script language="javascript" type="text/javascript">
/**
 * Processa o retorno da consulta do CEP
 * @param {string} retornoCEP Retorno da consulta do CEP com o endereço do indivíduo
 * @returns {undefined}
 * @see projevemurbano.js
 */
function processaRetornoCEP(retornoCEP) {
  var dadosEndereco = retornoCEP.split('||');
  jQuery('#polendereco').val(dadosEndereco[0]);
  jQuery('#polbairro').val(dadosEndereco[1]);
}
</script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="<?=$requisicao ?>">
<input type="hidden" name="polid" value="<?=$polid ?>">
<input type="hidden" name="aba" value="<?=$_GET['aba'] ?>">
<input type="hidden" name="munid" value="<?=$munid ?>">
<input type="hidden" name="pmuid" value="<?=$db->pegaUm("SELECT pmuid FROM projovemurbano.polomunicipio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND tprid = {$_SESSION['projovemurbano']['tprid']} AND pmustatus='A'") ?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações:</td>
		<td>
		<font color=blue>
		<p>O polo só pode ser constituído na localidade onde houver regional de ensino. 01 (um) em cada regional de educação que possua na sua área de abrangência município com núcleo do Programa.</p>
                <?php if (2 != $_SESSION['projovemurbano']['tprid']): ?>
		<p>Um núcleo é composto por 5 (cinco) turmas. Se o núcleo for igual a 1 (um), o nº de alunos deve ser necessariamente 200. Se o nº de núcleo for maior que 1 (um), o nº de alunos no núcleo poderá variar entre 150 e 200.</p>
		<p>A turma pode ter entre 30 e 40 alunos cada uma.</p>
                <?php else: ?>
                <p>O núcleo da Unidade Prisional é composto de no mínimo 60 estudantes e no máximo 150.</p>
                <?php endif; ?>
		</font>
		</td>
	</tr>
	<? if($pmupossuipolo=="t" && !$munid): ?>
	
	<? if($polid) : ?>
	<tr>
		<td class="SubTituloDireita" width="20%">Identificação do polo</td>
		<td><b>POLO <?=$polid ?></b></td>
	</tr>
	<? endif; ?>
	<tr>
		<td class="SubTituloDireita" width="20%">CEP</td>
		<td><? echo campo_texto('polcep', 'S', $habilita, 'CEP', 10, 9, "#####-###", "", '', '', 0, 'id="polcep"', '', mascaraglobal($polcep,'#####-###'), 'getEnderecoPeloCEP(this.value);' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Endereço</td>
		<td><? echo campo_texto('polendereco', 'S', $habilita, 'Endereço', 50, 200, "", "", '', '', 0, 'id="polendereco"', '', $polendereco, '' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Bairro</td>
		<td><? echo campo_texto('polbairro', 'S', $habilita, 'Bairro', 50, 200, "", "", '', '', 0, 'id="polbairro"', '', $polbairro, '' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Complemento</td>
		<td><? echo campo_texto('polcomplemento', 'N', $habilita, 'Complemento', 50, 200, "", "", '', '', 0, 'id="polcomplemento"', '', $polcomplemento, '' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Número</td>
		<td><? echo campo_texto('polnumero', 'S', $habilita, 'Número', 8, 7, "", "", '', '', 0, 'id="polnumero"', '', $polnumero, '' ); ?></td>
	</tr>
			
	<tr>
		<td class="SubTituloDireita">Telefone:</td>
		<td>( 
			<? echo campo_texto('poltelefoneddd', "N", $habilita, "DDD Telefone", 4, 2, "###", "", '', '', 0, 'id="poltelefoneddd"', '',  substr($poltelefone,0,2) ); ?> ) 
			<? echo campo_texto('poltelefone', "N", $habilita, "Telefone", 17, 15, "####-####", "", '', '', 0, 'id="poltelefone"', '', mascaraglobal(substr($poltelefone,2),"####-####") ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Email</td>
		<td><? echo campo_texto('polemail', 'N', $habilita, 'Email', 50, 200, "", "", '', '', 0, 'id="polemail"', '', $polemail, '' ); ?></td>
	</tr>
	<? endif; ?>
	<? if($munid) : ?>
	<tr>
	
		<td class="SubTituloDireita">UF</td>
		<td><?=$estuf ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Municipio</td>
		<td><?=$mundescricao ?> </td>
	</tr>
	<? if($polo) : ?> 
	<tr>
		<td class="SubTituloDireita">Identificador do polo</td>
		<td><b><?=$polo ?></b></td>
	</tr>
	<? endif; ?>
	<tr>
		<td class="SubTituloDireita">Nucleos</td>
		<td>
		<? if(!$habilitado) : ?>
		<p align="right"><input type="button" value="Inserir" onclick="gerenciarNucleos('<?=$munid ?>');"></p>
		<? endif; ?>
		<div id="div_mun_<?=$munid ?>"></div>
	
		<script>
		jQuery(document).ready(function() {
			carregarNucleos('<?=$munid ?>');
		});
		</script>
		
		</td>
	</tr>
	<? else : ?>
	<tr>
		<td class="SubTituloDireita">UF</td>
		<td><?=$estuf ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Municipio</td>
		<td>
		<?
		if($_SESSION['projovemurbano']['estuf']) {
			$sql = "SELECT muncod as codigo, REPLACE(mundescricao,'\'',' ') as descricao 
					FROM territorios.municipio  
					WHERE 
						estuf = '{$_SESSION['projovemurbano']['estuf']}' 
						AND muncod NOT IN
							(
							SELECT DISTINCT
								pju.muncod
							FROM
								projovemurbano.metasdoprograma mtp
							INNER JOIN projovemurbano.projovemurbano pju ON pju.pjuid = mtp.pjuid AND muncod IS NOT NULL AND pju.ppuid = 2
							INNER JOIN projovemurbano.tipometadoprograma tpm ON tpm.tpmid = mtp.tpmid
							WHERE
								tprid = {$_SESSION['projovemurbano']['tprid']}
								AND mtpvalor IS NOT NULL
							) 
					ORDER BY mundescricao";
// 			ver($sql,d);
			combo_popup( "muncod", $sql, "Municipios", "192x400", 0, array(), "", "S", false, false, 5, 400 );
		} elseif($_SESSION['projovemurbano']['muncod']) {
			$sql = " SELECT muncod as codigo, mundescricao as descricao FROM territorios.municipio 
					 WHERE 
						muncod='".$_SESSION['projovemurbano']['muncod']."' 
					ORDER BY mundescricao";
			$db->monta_combo('muncod[]', $sql, (($munid)?'N':'S'), 'Selecione', '', '', '', '200', 'S', 'muncod', '', $muncod );			
		}
//		ver($sql);
 
		
		?>
		</td>
	</tr>	
	<? endif; ?>
	<tr>
		<td class="SubTituloCentro" colspan="2">
		<? if(!$habilitado) : ?>
			<? if(!$munid & $habilita == 'S' ): ?>
				<input type="button" name="salvar" value="Salvar" onclick="gravarPoloNucleo();">
			<? endif; ?>
		<? endif; ?>
		<!-- 
				<input type="button" name="voltar" value="Voltar" onclick="window.location='projovemurbano.php?modulo=principalindexPoloNucleo2014&acao=A&aba=<?=$_GET['aba'] ?>&aba2=poloNucleo&aba3=poloNucleoCadastro';">
		 -->
		
		</td>
	</tr>
</table>
</form>
<? registarUltimoAcesso(); ?>
