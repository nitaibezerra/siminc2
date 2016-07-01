<?php

//verifica session
verificaSessao();


// ver($_SESSION,d);

$sql = "select docid from escolasexterior.escolasexterior where eexid = ".$_SESSION['eexid']; // Retorna o docid da escola
$docid = $db->pegaUm($sql);
$esdid = pegaEstadoAtual( $docid ); // Pega o estado atual da escola

//verifica permissão
$vPermissao = 'N';
$vpflcod = arrayPerfil();
if($esdid == WF_ESDID_EM_CADASTRAMENTO && (in_array(PERFIL_SUPER_USUARIO, $vpflcod) || in_array(PERFIL_CADASTRADOR, $vpflcod) || in_array(PERFIL_ADMINISTRADOR, $vpflcod))){
	$vPermissao = 'S';
}

/*
$docid = criaDocumento( $_SESSION['entid'], $_SESSION['emiid']   );
$esdid = pegaEstadoAtual( $docid );

/*
$ativo = 'N';
if(checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_CADASTRADOR)) && in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO, false))){
	$ativo = 'S';	
}
*/


//submit
if($_POST){
	
	//verifica se ja existe a escola
		$sql = "select count(eexid) as total 
				from escolasexterior.escolasexterior 
				where eexstatus = 'A' 
				and eexnomeestabelecimento = '".strtoupper($_POST['eexnomeestabelecimento'])."'
				and eexid not in (".$_SESSION['eexid'].")";
		$verifica = $db->pegaUm($sql);
		
		if( (int) $verifica > 0 ){
			print "<script>
					alert('Erro: Escola já existe!');
					history.back();
			   	  </script>";
			exit();
		}
	//fim verifica
	
	$sql = "UPDATE 
				escolasexterior.escolasexterior
			SET 
				eexnomeestabelecimento = '".$_POST['eexnomeestabelecimento']."',
				eexcidade = '".$_POST['eexcidade']."',
				eexendereco = '".$_POST['eexendereco']."',
				eextelefone = ".($_POST['eextelefone'] ? "'".$_POST['eextelefone']."'" : 'null').",
				eexfax = ".($_POST['eexfax'] ? "'".$_POST['eexfax']."'" : 'null').",
				eexemail = ".($_POST['eexemail'] ? "'".$_POST['eexemail']."'" : 'null').",
				eexmantenedor = ".($_POST['eexmantenedor'] ? "'".$_POST['eexmantenedor']."'" : 'null').",
				eexcoordenador = ".($_POST['eexcoordenador'] ? "'".$_POST['eexcoordenador']."'" : 'null').",
				eexdiretor = ".($_POST['eexdiretor'] ? "'".$_POST['eexdiretor']."'" : 'null').",
				eexprocessoconjur = ".($_POST['eexprocessoconjur'] ? "'".$_POST['eexprocessoconjur']."'" : 'null').",
				eexdatainauguracao = ".($_POST['eexdatainauguracao'] ? "'".formata_data_sql($_POST['eexdatainauguracao'])."'" : 'null').",
				eexlatitude = ".($_POST['endereco'][1]['medlatitude'] ? "'".(implode('.',$_POST['endereco'][1]['medlatitude']))."'" : 'null').",
				eexlongitude = ".($_POST['endereco'][1]['medlongitude'] ? "'".(implode('.',$_POST['endereco'][1]['medlongitude']))."'" : 'null').",
				endzoom = ".($_POST['endzoom'] ? $_POST['endzoom'] : 'null').",
				eexcep = ".($_POST['eexcep'] ? "'".$_POST['eexcep']."'" : 'null').",
				eexbairro = ".($_POST['eexbairro'] ? "'".$_POST['eexbairro']."'" : 'null').",
				eexestuf = ".($_POST['eexestuf'] ? "'".$_POST['eexestuf']."'" : 'null').",
				paiid = ".($_POST['paiid'] ? "'".$_POST['paiid']."'" : 'null')."
			WHERE 
				eexid = ".$_SESSION['eexid'];
	// ver($sql,d);
	$db->executar($sql);
	$db->commit();


	// associa cargos
	foreach ($_POST['etapaensino'] as $key => $value) {
		$sql1 = "
			SELECT *
			FROM escolasexterior.etapaensinoescola
			WHERE eexid = '{$_SESSION['eexid']}' AND eteid = '{$value}'
		";
		// ver($sql1,d);
		$busca = $db->carregar( $sql1 );
		if( !$busca ){
			$sql = "
				INSERT INTO escolasexterior.etapaensinoescola
				(
					eexid,
					eteid
				) VALUES (
					'{$_SESSION['eexid']}',
					'{$value}'
				)
			";
		}else{
			$sql = "
				UPDATE escolasexterior.etapaensinoescola
				SET etpstatus = 'A'
				WHERE eexid = '{$_SESSION['eexid']}' AND eteid = '{$value}'
			";
		}
		// ver($sql,d);
		$db->executar( $sql );
		$db->commit();
	}
	// desativa nao presentes
	if( $_POST['etapaensino'] ){
		$sql = "
			UPDATE escolasexterior.etapaensinoescola
			SET etpstatus = 'I'
			WHERE eexid = '{$_SESSION['eexid']}' and eteid NOT IN ('".(implode("','",$_POST['etapaensino']))."')
		";
		// ver($sql,d);
		$db->executar( $sql );
		$db->commit();
	}
	// --
	
	unset($_POST);
	
	echo '<script>
			alert("Operação realizada com sucesso!");
			location.href="escolasexterior.php?modulo=principal/dadosEscola&acao=A";
		   </script>';
	exit();
	
}



// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

$titulo = "ESCOLAS NO EXTERIOR";

$dsctitulo = 'CADASTRO - DADOS DA ESCOLA<BR><img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.';



montaAbasEscolasExterior();


monta_titulo( $titulo, $dsctitulo );

//cabecalhoEE($_SESSION['eexid']);


//carrega dados para edição
if($_SESSION['eexid']){
	$sql = "SELECT
				  	eexid,
					paiid,
				  	eexnomeestabelecimento,
				  	eexcidade,
				  	eexendereco,
				  	eextelefone,
				  	eexfax,
				  	eexemail,
				  	eexmantenedor,
				  	eexprocessoconjur,
				  	eexdatainauguracao,
				  	eexdatainclusao,
				  	eexexercicio,
				  	eexstatus,
				  	eexlatitude,
				  	eexlongitude,
				  	endzoom,
				  	eexestuf,
				  	eexcep, 
				  	eexbairro,
				  	eexestuf,
				  	eexdiretor,
				  	eexcoordenador
		     FROM 
		     		escolasexterior.escolasexterior
		     WHERE 
		     		eexid = {$_SESSION['eexid']}";
	$dados = $db->carregar($sql);
	if($dados) extract($dados[0]);
	
	$latitude = explode(".",$eexlatitude);
	
	if(trim($eexlatitude) != ''){
		//trata para não aparecer XX quando for 0.
		$latitude[0] = $latitude[0] == 0 ? '00' : $latitude[0];
		$latitude[1] = $latitude[1] == 0 ? '00' : $latitude[1];
		$latitude[2] = $latitude[2] == 0 ? '00' : $latitude[2];
	}
	
	$longitude = explode(".",$eexlongitude);
	
	if(trim($eexlongitude) != ''){
		//trata para não aparecer XX quando for 0.
		$longitude[0] = $longitude[0] == 0 ? '00' : $longitude[0];
		$longitude[1] = $longitude[1] == 0 ? '00' : $longitude[1];
		$longitude[2] = $longitude[2] == 0 ? '00' : $longitude[2];
	}
}

?>

<script src="../includes/calendario.js"></script>

<script type="text/javascript" src="/includes/entidadesn.js"></script>


<form id="formulario" name="formulario" action="" method="post">

<input type="hidden" name="entid" id="entid">

<table class="Tabela" align="center"  cellpadding="2" cellspacing="1">
	
	<tr>
		<td width="32%" class="SubTituloDireita" colspan='2'>Nome da Escola:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexnomeestabelecimento', 'S', $vPermissao, '', 100, 100, '', '', '', '', 0, '');?>
		</td>
		<td align="right" rowspan="13">
			
				<?php
					if($_SESSION['eexid']){
						$docid = criaDocumento( $_SESSION['eexid'] ); 
						if($docid){
							// ver($docid,d);
							// envioEmailGenericoEscolasExterior( $docid );exit('teste');
							$esdid = wf_pegarEstadoAtual( $docid );
							wf_desenhaBarraNavegacao( $docid , array("eexid" => $_SESSION['eexid'], "docid"=>$docid) );
						} 
					}
				?>
			
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>País:</td>
		<td colspan='2'>
			<?
				$sql_combo = "SELECT
								paiid as codigo,
								paidescricao as descricao
							  FROM territorios.pais
							  -- where paiid = {$paiid}
							  order by 2";
				$db->monta_combo("paiid", $sql_combo, $vPermissao, "", '', '', '', '', 'S', 'paiid1');
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Estado:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexestuf', 'N', $vPermissao, '', 100, 100, '', '', '', '', 0, 'id="estuf1"');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Cidade:</td>
		<td colspan='2'>
			<input type="hidden" id="muncod1"/>
			<?=campo_texto( 'eexcidade', 'S', $vPermissao, '', 100, 100, '', '', '', '', 0, 'id="mundescricao1"');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Endereço:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexendereco', 'S', $vPermissao, '', 100, 100, '', '', '', '', 0, '');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Bairro:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexbairro', 'N', $vPermissao, '', 100, 100, '', '', '', '', 0, 'id="endbai1"');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>CEP:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexcep', 'S', $vPermissao, '', 100, 100, '', '', '', '', 0, 'id="endcep1"');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Telefone:</td>
		<td colspan='2'>
			<?=campo_texto( 'eextelefone', 'S', $vPermissao, '', 15, 15, '', '', '', '', 0, '');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Fax:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexfax', 'N', $vPermissao, '', 10, 10, '', '', '', '', 0, '');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>E-mail:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexemail', 'S', $vPermissao, '', 50, 50, '', '', '', '', 0, '');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Mantenedor:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexmantenedor', 'S', $vPermissao, '', 100, 100, '', '', '', '', 0, '');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Coordenador Pedagógico:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexcoordenador', 'S', $vPermissao, '', 100, 100, '', '', '', '', 0, '');?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Diretor:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexdiretor', 'S', $vPermissao, '', 100, 100, '', '', '', '', 0, '');?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Etapas de ensino:</td>
		<td colspan='2'>
			<?
				$sql_combo = "SELECT
								eteid as codigo,
								etedsc as descricao
							  FROM 
							  	escolasexterior.etapaensino
							  WHERE etstatus='A'
							  ORDER BY 2";
				$etapaensino = array();
				if( $_SESSION['eexid'] ){
					$sql_carregados = "
						SELECT
							e.eteid as codigo,
							e.etedsc as descricao
						FROM 
						  	escolasexterior.etapaensinoescola eee
						JOIN escolasexterior.etapaensino e ON eee.eteid = e.eteid
						WHERE 
							eee.etpstatus='A' 
							AND eee.eexid={$_SESSION['eexid']}
						ORDER BY 2
					";
					// ver($sql_carregados,d);
					$etapaensino = $db->carregar( $sql_carregados );
				}
				combo_popup( 'etapaensino', $sql_combo, 'Etapa de Ensino: ','400x400',0,$etapaensino,'',$vPermissao);
			?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Processo Conjur:</td>
		<td colspan='2'>
			<?=campo_texto( 'eexprocessoconjur', 'N', $vPermissao, '', 20, 20, '', '', '', '', 0, '');?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" colspan='2'>Data de Inauguração:</td>
		<td colspan='2'>
			<?=campo_data('eexdatainauguracao', 'S', $vPermissao, '', 'S' );?>
		</td>
	</tr>
		<tr>  
		<td class="SubTituloDireita" colspan='2'>Latitude:</td>
		<td colspan='2'>
			<? echo "<span id=\"_graulatitude1\">".((trim($latitude[0]))?trim($latitude[0]):"XX")."</span>º 
							  	  	  <span id=\"_minlatitude1\">". ((trim($latitude[1]))?trim($latitude[1]):"XX")."</span>' 
							  	  	  <span id=\"_seglatitude1\">". ((trim($latitude[2]))?trim($latitude[2]):"XX")."</span>\" 
							  	  	  <span id=\"_pololatitude1\">".((trim($latitude[3]))?trim($latitude[3]):"XX")."</span>
							  	  	  <input type=\"hidden\" name=\"endereco[1][medlatitude][0]\" id=\"graulatitude1\" maxlength=\"2\" size=\"3\" value=\"".trim($latitude[0])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[1][medlatitude][1]\" id=\"minlatitude1\" size=\"3\" maxlength=\"2\" value=\"".trim($latitude[1])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[1][medlatitude][2]\" id=\"seglatitude1\" size=\"3\" maxlength=\"2\" value=\"".trim($latitude[2])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[1][medlatitude][3]\" id=\"pololatitude1\" value=\"".trim($latitude[3])."\">";
			?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'>Longitude:</td>
		<td colspan='2'>
			<? echo "<span id=\"_graulongitude1\">".((trim($longitude[0]))?trim($longitude[0]):"XX")."</span>º
							  	  	  <span id=\"_minlongitude1\">".((trim($longitude[1]))?trim($longitude[1]):"XX")."</span>' 
							  	  	  <span id=\"_seglongitude1\">".((trim($longitude[2]))?trim($longitude[2]):"XX")."</span>\" 
							  	  	  <span id=\"_pololongitude1\">".((trim($longitude[3]))?trim($longitude[3]):"XX")."</span>
							  	  	  <input type=\"hidden\" name=\"endereco[1][medlongitude][0]\" id=\"graulongitude1\" maxlength=\"2\" size=\"3\" value=\"".trim($longitude[0])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[1][medlongitude][1]\" id=\"minlongitude1\" size=\"3\" maxlength=\"2\" value=\"".trim($longitude[1])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[1][medlongitude][2]\" id=\"seglongitude1\" size=\"3\" maxlength=\"2\" value=\"".trim($longitude[2])."\" class=\"normal\" onKeyUp=\"this.value=mascaraglobal('##',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"> 
							  	  	  <input type=\"hidden\" name=\"endereco[1][medlongitude][3]\" id=\"pololongitude1\" value=\"".trim($longitude[3])."\">";
			?>
		</td>
	</tr>
	<tr>  
		<td class="SubTituloDireita" colspan='2'></td>
		<td colspan='2'>
			<? 
				echo "<a href=\"#\" onclick=\"abreMapaEntidade('1');\">Visualizar / Buscar No Mapa</a> <input style=\"display:none;\" type=\"text\" name=\"endzoom\" id=\"endzoom1\" value=\"".$endzoom."\">";
			?>
		</td>
	</tr>
	<tr>
		<td align="center" width="100%" style="background: rgb(238, 238, 238)" colspan='3'>
			<table width="100%">
				<tr>
					<td width="55%" align="right">
						<?if($vPermissao == 'S'){?>
							<input type="button" class="botao" name="btSalvar" id="btSalvar" value="Salvar" onclick="validaFormE();">
						<?}?>
						&nbsp;
						<input type="button" class="botao" name="btLimpar" id="btLimpar" value="Limpar" onclick="location.href='escolasexterior.php?modulo=principal/cadastroEscola&acao=A&eexid=<?=$_REQUEST['eexid']?>';">
					</td>
					<td align="right">&nbsp;</td>
				</tr>
			</table>
		</td>
	</tr>
	
</table>


</form>

</body>
</html>

 
<script>

var d = document.formulario;



function validaFormE(){

	if(!validaBranco( d.eexnomeestabelecimento, 'Nome da Escola' ))
	{
		return false;
	}
	if(!validaBranco( d.eexcidade, 'Cidade' ))
	{
		return false;
	}
	if(!validaBranco( d.eexendereco, 'Endereço' ))
	{
		return false;
	}
	if(!validaBranco( d.eextelefone, 'Telefone' ))
	{
		return false;
	}
	if(!validaBranco( d.eexemail, 'E-mail' ))
	{
		return false;
	}
	if(!validaBranco( d.eexmantenedor, 'Mantenedor / Responsável Legal' ))
	{
		return false;
	}
	if(!validaBranco( d.eexcoordenador, 'Coordenador Pedagógico' ))
	{
		return false;
	}
	if(!validaBranco( d.eexdiretor, 'Diretor' ))
	{
		return false;
	}
	
	if ( !document.getElementById('etapaensino').options[0].value ){
			alert('Campo obrigatório: Etapas de ensino.');
			return false;
	}
	
	if(!validaBranco( d.eexdatainauguracao, 'Data de Inauguração' ))
	{
		return false;
	}	

	selectAllOptions( document.getElementById( 'etapaensino' ) );
		
	d.submit();

}


</script>