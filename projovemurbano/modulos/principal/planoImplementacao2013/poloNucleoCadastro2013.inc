<?

$sql = "SELECT * 
		FROM projovemurbano.polomunicipio 
		WHERE 
			pjuid='".$_SESSION['projovemurbano']['pjuid']."' 
			AND tprid = {$_SESSION['projovemurbano']['tprid']} AND pmustatus='A'";

$polomunicipio = $db->pegaLinha($sql);

if($polomunicipio) {
	
	extract($polomunicipio);
	$requisicao = "atualizarPoloMunicipio";
	
} else {
	
	$requisicao = "inserirPoloMunicipio";
}

$habilButton = "S";
$perfil = pegaPerfilGeral();
if( in_array(PFL_CONSULTA, $perfil) ){
	$habilButton = "N";
}

if($esdid!=ESD_EMELABORACAO && !$db->testa_superuser()) {
	$habilitado = "disabled";
	$habilButton = "N";
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="./js/projovemurbano.js"></script>
<form id="form" name="form" method="POST">
<input type="hidden" name="requisicao" value="<?=$requisicao ?>">
<input type="hidden" name="pmuid" value="<?=$pmuid ?>">
<input type="hidden" name="aba" value="<?=$_GET['aba'] ?>">

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="20%">Orientações:</td>
		<td>
			<font color=blue>
				<?=$arrOrientacoes[3] ?>
			</font>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Seu <?=(($_SESSION['projovemurbano']['estuf'])?"estado":"município") ?> possui regional?</td>
		<td>
			<input type="radio" <?=$habilitado ?> name="pmupossuipolo" value="TRUE"  <?=(($pmupossuipolo=='t')?"checked":"") ?> onclick="divCarregando();jQuery('#form').submit();" > Sim 
			<input type="radio" <?=$habilitado ?> name="pmupossuipolo" value="FALSE" <?=(($pmupossuipolo=='f')?"checked":"") ?> onclick="divCarregando();jQuery('#form').submit();" > Não
		</td>
	</tr>
	
	<? if($pmupossuipolo=='f') : ?>
	<tr>
		<td colspan="2">
		<? if(!$habilitado && $habilButton == 'S') : ?>
		<p><input type="button" value="Adicionar Município/Núcleo" 
					onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?=$_GET['aba'] ?>&aba2=poloNucleo&aba3=poloNucleoGerenciar';"></p>
		<? endif; ?>
		
		<?
		$sql = "SELECT '<img src=../imagens/gif_inclui.gif style=cursor:pointer; 
							 onclick=\"window.location=\'projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoGerenciar&munid='||muni.munid||'\';\">".((!$habilitado)?" 
						<img src=../imagens/excluir.gif style=cursor:pointer; 
							 onclick=\"excluirPoloMunicipio(\'projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro&requisicao=excluirMunicipio&munid='||muni.munid||'\');\">":"")."' as acao, 
					   mun.estuf||' / '||mun.mundescricao as loc,
					   (SELECT count(*) FROM projovemurbano.nucleo nuc WHERE nuc.munid=muni.munid AND muni.munstatus='A' AND nuc.nucstatus='A') as qtdnucleo
				FROM projovemurbano.polomunicipio plm 
				INNER JOIN projovemurbano.municipio muni ON muni.pmuid = plm.pmuid 
				INNER JOIN territorios.municipio mun ON mun.muncod = muni.muncod   
				WHERE plm.pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND plm.tprid = {$_SESSION['projovemurbano']['tprid']} AND muni.munstatus='A'";
		
		$cabecalho = array("&nbsp;","UF/Município","Quantidade núcleos");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		?>
		</td>
	</tr>
	<? endif; ?>
	
	<? if($pmupossuipolo=='t') : ?>
	<tr>
		<td colspan="2">
		<? if(!$habilitado) : ?>
		<p>
			<input type="button" value="Adicionar Polo" 
					onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?=$_GET['aba'] ?>&aba2=poloNucleo&aba3=poloNucleoGerenciar';"></p>
		<? endif; ?>
		
		<?
		echo "<h3>Lista de polos</h3>";
		$sql = "SELECT '<img src=../imagens/alterar.gif style=cursor:pointer; 
								onclick=\"window.location=\'projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoGerenciar&polid='||pol.polid||'\';\"> ".((!$habilitado)?" 
						<img src=../imagens/excluir.gif style=cursor:pointer; 
								onclick=\"excluirPoloMunicipio(\'projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro&requisicao=excluirPolo&polid='||pol.polid||'\');\">":"")."' as acao,
					   '<center><b>POLO '||pol.polid||'</b></center>' as identificacao, 
					   'CEP: '||COALESCE(pol.polcep::text,'')||', '||COALESCE(pol.polendereco,'')||' '||COALESCE(pol.polcomplemento,'')||', '||COALESCE(pol.polbairro,'')||', nº '||COALESCE(pol.polnumero::text,'') as endereco, 
					   (SELECT count(*) FROM projovemurbano.municipio muni 
					   	INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = muni.munid 
					   	WHERE amp.polid=pol.polid AND muni.munstatus='A') as qtdmunicipios
				FROM projovemurbano.polomunicipio plm 
				INNER JOIN projovemurbano.polo pol ON pol.pmuid = plm.pmuid 
				WHERE plm.pjuid='".$_SESSION['projovemurbano']['pjuid']."' AND pol.polstatus='A'";
//		ver($sql);
		$cabecalho = array("&nbsp;","Identificação","Endereço do polo","Quantidade municípios");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'S','100%',$par2);
        
		if(!$habilitado){
			$excluir = "<img src=../imagens/excluir.gif style=cursor:pointer; 
									onclick=\"excluirPoloMunicipio(\'projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoCadastro&requisicao=excluirMunicipio&munid='||muni.munid||'\');\">";
		}
		echo "<h3>Lista de municipios</h3>";		
		$sql = "SELECT '<img src=../imagens/gif_inclui.gif style=cursor:pointer; 
								onclick=\"window.location=\'projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba={$_GET['aba']}&aba2=poloNucleo&aba3=poloNucleoGerenciar&munid='||muni.munid||'\';\"> 
						$excluir' as acao, 
					   '<center><b>POLO '||pol.polid||'</b></center>' as identificacao, 
					   mun.estuf||' / '||mun.mundescricao as loc,
					   (SELECT count(*) FROM projovemurbano.nucleo nuc WHERE nuc.munid=muni.munid AND nuc.tprid = {$_SESSION['projovemurbano']['tprid']} AND nuc.nucstatus='A') as qtdnucleo
				FROM projovemurbano.polomunicipio plm 
				INNER JOIN projovemurbano.polo pol ON pol.pmuid = plm.pmuid 
				INNER JOIN projovemurbano.associamucipiopolo amp ON amp.polid = pol.polid 
				INNER JOIN projovemurbano.municipio muni ON muni.munid = amp.munid  
				INNER JOIN territorios.municipio mun ON mun.muncod = muni.muncod   
				WHERE plm.pjuid='".$_SESSION['projovemurbano']['pjuid']."' 
                    --AND plm.tprid = {$_SESSION['projovemurbano']['tprid']}
                    AND pol.polstatus='A' AND muni.munstatus='A'
				ORDER BY
					2";
//		ver($sql);
		$cabecalho = array("&nbsp;","Identificação do polo","UF/Município","Quantidade núcleos");
		$db->monta_lista_simples($sql,$cabecalho,50,5,'N','100%',$par2);
		?>
		</td>
	</tr>
	<? endif; ?>
	<tr>
		<td class="SubTituloCentro" colspan="2">
			<input type="button" name="anterior" value="Anterior" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?=$_GET['aba'] ?>&aba2=metaMatriculaInicioAula';"> 
			<input type="button" name="proximo" value="Próximo" onclick="window.location='projovemurbano.php?modulo=principal/planoImplementacao2013&acao=A&aba=<?=$_GET['aba'] ?>&aba2=poloNucleo&aba3=poloNucleoResumo';">
			
		</td>
	</tr>

</table>
</form>
<? registarUltimoAcesso(); ?>
