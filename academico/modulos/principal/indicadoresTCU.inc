<?
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
$anoref = $_REQUEST['anoref'] ? $_REQUEST['anoref'] : date('Y')-1;

if($_REQUEST["submetido"]) {
	$quantIndicador = $db->pegaUm("SELECT count(*) FROM academico.indicadorestcu WHERE tcuid NOT IN (20,21)");

	for($i=1; $i<=$quantIndicador; $i++) {
		$sql = "SELECT count(*) FROM academico.lancamentoindicador WHERE tcuid = ".$_REQUEST['id_indicador_'.$i]." AND mviid = ".$_REQUEST['mviid']." AND lciano = '{$anoref}'";
		$existeLancamentoIndicador = $db->pegaUm($sql);
		
		$lcivalor 	=	(($_REQUEST["vlr_indicador_$i"] != "") ? $_REQUEST["vlr_indicador_$i"] : 0);
		$lciobs		=	(($_REQUEST["obs_indicador_$i"] != "") ? "'" . simec_htmlspecialchars($_REQUEST["obs_indicador_$i"], ENT_QUOTES) . "'" : "NULL");
		
		$lcivalor = str_replace('.', '',$lcivalor);
		$lcivalor = trim(str_replace(',', '.',$lcivalor));

		if($existeLancamentoIndicador > 0) {
			$sql = "UPDATE
						academico.lancamentoindicador
					SET
						lciobs = {$lciobs},
						lcivalor = {$lcivalor}
					WHERE
						tcuid = {$_REQUEST["id_indicador_$i"]} AND
						mviid = {$_REQUEST["mviid"]} AND
						lciano = '{$anoref}'";
		} else {
			$sql = "INSERT INTO academico.lancamentoindicador
						(tcuid, mviid, usucpf, lciobs, lcivalor, lcidtinclusao, lciano)
					VALUES
						({$_REQUEST["id_indicador_$i"]}, {$_REQUEST["mviid"]}, '{$_SESSION["usucpf"]}', {$lciobs}, {$lcivalor}, now(), '{$anoref}')";
		}
		
		$db->executar($sql);
	}

	$db->commit();
	die("<script>
			alert('Dados gravados com sucesso.');
			window.location.replace( window.location );
		 </script>");
}

// recupera o entid
$entid = academico_existeentidade( $_SESSION["academico"]["entid"] );

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

if( !$entid ){
	echo "<script>
			alert('A Unidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

// Monta as abas
#$db->cria_aba($abacod_tela, $url, $parametros);
monta_titulo("Indicadores do TCU", "");

// cria o cabeçalho padrão do módulo
$autoriazacaoconcursos = new autoriazacaoconcursos();
#echo "<br />";

// recupera o identificador da tabela 'movimentoindicador'
$mviid = recuperaMovimentoIndicador( $entid );

$perfilNotBloq = array( PERFIL_SUPERUSUARIO,
						PERFIL_IFESCADTCU);

						
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
//ver(1,1,1,$permissoes,d);

validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança

$ano = date(Y)-1;
$primeiroano = 2009;
$block = false;
if( $anoref != $ano && !academico_possui_perfil( PERFIL_ADMINISTRADOR ) ){
	$block = true;
}
$bloqueado = $permissoes['gravar']==1 && !$block ? "" :  "disabled";

if($bloqueado == 'disabled'){
	$inputTCU = "<input type=\'text\' disabled size=\'20\' class=\'normal\' name=\'vlr_indicador_' || tcu.tcuid || '\' style=\'text-align:right\' onkeyUp=\'this.value=mascaraglobal(\"###.###.###.###,##\",this.value);\' onkeyPress=\'return somenteNumeros(event);\' value=\'' || trim(to_char(coalesce(lci.lcivalor, 0),'999G999G999G990D99')) || '\' /><input type=\'hidden\' name=\'vlr_indicador_' || tcu.tcuid || '\' value=\'' || trim(to_char(coalesce(lci.lcivalor, 0),'999G999G999G990D99')) || '\' />";
}else{
	$inputTCU = "<input type=\'text\' size=\'20\' class=\'normal\' name=\'vlr_indicador_' || tcu.tcuid || '\' style=\'text-align:right\' onkeyUp=\'this.value=mascaraglobal(\"###.###.###.###,##\",this.value);\' onkeyPress=\'return somenteNumeros(event);\' value=\'' || trim(to_char(coalesce(lci.lcivalor, 0),'999G999G999G990D99')) || '\' />";
}
$inputTCULivre = "<input type=\'text\' size=\'20\' class=\'normal\' name=\'vlr_indicador_' || tcu.tcuid || '\' style=\'text-align:right\' onkeyUp=\'this.value=mascaraglobal(\"###.###.###.###,##\",this.value);\' onkeyPress=\'return somenteNumeros(event);\' value=\'' || trim(to_char(coalesce(lci.lcivalor, 0),'999G999G999G990D99')) || '\' />";

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
<!--
	$(document).ready(function(){
		$('#anoref').change(function(){
			document.location.href = '?modulo=principal/indicadoresTCU&acao=A&anoref='+this.value;
		});
	});
//-->
</script>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <? echo $autoriazacaoconcursos->cabecalho_entidade($entid); ?>
<center>
<b>Ano de referência</b>
<br/> 
<select id="anoref" name="anoref">
<?php 
	for($x = $ano; $x>=2006;$x--  ){
?>
	<option value="<?=$x ?>" <?php echo ($_GET['anoref'] == $x ) ? "selected" : ""?>><?=$x ?></option>
<?php 
	}
?>
</select>
</center>
<br/>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<form action="" method="post" name="formIndicadoresTCU" id="formIndicadoresTCU">
<input type="hidden" name="mviid" id="mviid" value="<?=$mviid?>" />
<input type="hidden" name="anoref" id="anoref" value="<?=$anoref?>" />
<input type="hidden" name="submetido" id="submetido" value="1" />
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloEsquerda" style="text-align:center;"><b>COMPONENTES</b></td>
		</tr>
		<tr>
			<td>
			<?
				$sql = "SELECT
							tcu.tcucodigo || '<input type=\'hidden\' id=\'id_indicador_' || tcu.tcuid || '\' name=\'id_indicador_' || tcu.tcuid || '\' value=\'' || tcu.tcuid || '\' />' as codigo,
							tcu.tcudsc as descricao,
							CASE WHEN tcu.tcuindvlr = 'R' 
								 THEN 'R$ ".$inputTCU."'
								 ELSE '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$inputTCU."'
							END as valor,
							'<img src=\'../imagens/edit_on.gif\' style=\'cursor:pointer;\' onclick=\'popupObservacao(' || tcu.tcuid || ');\' />
							 <input type=\'hidden\' name=\'obs_indicador_' || tcu.tcuid || '\' id=\'obs_indicador_' || tcu.tcuid || '\' value=\"'|| coalesce(lci.lciobs, '') ||'\" />' as observacao
						FROM
							academico.indicadorestcu tcu
						LEFT JOIN academico.lancamentoindicador lci ON lci.tcuid = tcu.tcuid
																	   AND lci.mviid = {$mviid}
																	   AND lciano = '{$anoref}'
						WHERE
							tcu.tcutipo = 'C'";
				
				$cabecalho 		= array('Código', 'Descrição', 'Valor', 'Observação');				
				$tamanho		= array('10%', '60%', '20%', '10%');															
				$alinhamento	= array('left', 'left', 'right', 'center');													
				$db->monta_lista_simples($sql,$cabecalho,999,10,'N','100%', '', '');
			?>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloEsquerda" style="text-align:center;"><b>INDICADORES</b></td>
		</tr>
		<tr>
			<td>
			<?
				$sql = "SELECT
							tcu.tcucodigo || '<input type=\'hidden\' id=\'id_indicador_' || tcu.tcuid || '\' name=\'id_indicador_' || tcu.tcuid || '\' value=\'' || tcu.tcuid || '\' />' as codigo,
							tcu.tcudsc as descricao,
							CASE WHEN tcu.tcuid in(20,21) THEN 
								CASE WHEN tcu.tcuindvlr = 'R' 
									 THEN 'R$ ".$inputTCULivre."'
									 ELSE '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$inputTCULivre."'
								END
							ELSE
								CASE WHEN tcu.tcuindvlr = 'R' 
									 THEN 'R$ ".$inputTCU."'
									 ELSE '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;".$inputTCU."'
								END
							END AS valor,
							'<img src=\'../imagens/edit_on.gif\' style=\'cursor:pointer;\' onclick=\'popupObservacao(' || tcu.tcuid || ');\' />
							 <input type=\'hidden\' name=\'obs_indicador_' || tcu.tcuid || '\' id=\'obs_indicador_' || tcu.tcuid || '\' value=\''|| coalesce(lci.lciobs, '') ||'\' />' as observacao
						FROM
							academico.indicadorestcu tcu
						LEFT JOIN academico.lancamentoindicador lci ON lci.tcuid = tcu.tcuid 
																       AND lci.mviid = {$mviid}
																       AND lciano = '{$anoref}'
						WHERE
							tcu.tcutipo = 'I'
						AND
							tcu.tcuid NOT IN (20,21)
						ORDER BY
							tcu.tcuid";
				//ver($sql, 1);
				$cabecalho 		= array('Código', 'Descrição', 'Valor', 'Observação');				
				$tamanho		= array('10%', '60%', '20%', '10%');															
				$alinhamento	= array('left', 'left', 'right', 'center');													
				$db->monta_lista_simples($sql,$cabecalho,999,10,'N','100%', '', '');
			?>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td bgcolor="#c0c0c0" style="text-align:center;">
				<input type="button" id="bt_salvar" <?= $bloqueado; ?> value="Salvar" onclick="salvarIndicadores();" />
				<input type="button" id="bt_voltar" value="Voltar" onclick="javascript:history.back(-1);" />
			</td>
		</tr>
	</table>
</form>
<table class="tabela" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td style="text-align:right;"><b><a href="/academico/arquivos/manualIndTCU.pdf" target="_blank"><font color='red'>Manual de Orientações para o Cálculo dos Indicadores de Gestão</font></b></a></td>
	</tr>
</table>
</div>
<script type="text/javascript">

function salvarIndicadores() {
	var btVoltar 	= document.getElementById("bt_voltar");
	var btSalvar 	= document.getElementById("bt_salvar");
	var form 		= document.getElementById("formIndicadoresTCU");

	btVoltar.disabled = true;
	btSalvar.disabled = true;

	form.submit();
	
}

function popupObservacao(tcuid) {
	var janela = window.open("?modulo=principal/popObsIndicadoresTCU&acao=A&tcuid="+tcuid, "observacao", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=600,height=350");
	janela.focus();
}

</script>