<?
// transforma consulta em pública
if ( $_POST['prtid'] && $_POST['publico'] ){
	$sql = sprintf(
		"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
		$_POST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	$db->sucesso('principal/tabela');
	die;
}
// FIM transforma consulta em pública

// remove consulta
if ( $_POST['prtid'] && $_POST['excluir'] == 1 ) {
	$sql = sprintf(
		"DELETE from public.parametros_tela WHERE prtid = %d",
		$_POST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	$db->sucesso('principal/tabela');
	die;
}

if ( $_REQUEST['requisicao'] == 'salvarIndicadores' ){
	$existe_rel = 0;
	
	$sql = sprintf(
		"select prtid from public.parametros_tela where prtdsc = '%s'",
		$_POST['titulo_indicadores']
	);
	$existe_rel = $db->pegaUm( $sql );
	if ($existe_rel > 0) 
	{
		$sql = sprintf(
			"UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
			$_POST['titulo_indicadores'],
			addslashes( addslashes( serialize( $_POST['indicador'] ) ) ),
			$_SESSION['usucpf'],
			$_SESSION['mnuid'],
			$existe_rel
		);
		$db->executar( $sql );
		$db->commit();
	}
	else 
	{
		$sql = sprintf(
			"INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
			$_POST['titulo_indicadores'],
			addslashes( addslashes( serialize( $_POST['indicador'] ) ) ),
			'FALSE',
			$_SESSION['usucpf'],
			$_SESSION['mnuid']
		);
		$db->executar( $sql );
		$db->commit();
	}
	$db->sucesso('principal/tabela');
	die;
}

if ( $_POST['prtid'] && $_POST['carregar'] == 1 ) {
	if ( $_POST['prtid'] ){
		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_POST['prtid'] );
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	}
}

if($_REQUEST['resetar'] && $db->testa_superuser()){
	$sql = "update public.arquivo
			set arqstatus = 'I'
		where
			sisid = {$_SESSION['sisid']}
		and
			arqstatus = 'A'
		and
			arqextensao = 'html'";
	$db->executar($sql);
	$db->commit($sql);
	echo "<script>window.location.href='painel.php?modulo=principal/tabela&acao=A'</script>";
	exit;
}

if($_REQUEST['buscaMunicipioAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	buscaMunicipioAjax($_REQUEST['buscaMunicipioAjax']);
	exit;
}

function buscaMunicipioAjax($dsc){
	// carrega as funções gerais
	include_once "config.inc";
	include_once APPRAIZ . "includes/funcoes.inc";
	include_once APPRAIZ . "includes/classes_simec.inc";

	// carrega as funções específicas do módulo
	include_once '_constantes.php';
	include_once '_funcoes.php';
	include_once '_componentes.php';

	// abre conexão com o servidor de banco de dados
	$db = new cls_banco();

	$arrCaracteres = array(",",".","-",":","_","/");

	foreach($arrCaracteres as $crt){
		if(strstr($dsc, $crt)){
			$uf = strstr($dsc, $crt);
			$part = explode($crt,$dsc);
			$mun = trim($part[0]);
			$mun = str_replace($crt,'',$mun);
			$mun = trim($mun);
			$uf = str_replace($crt,'',$uf);
			$uf = trim($uf);
			break;
		}

	}

	if(!$mun){
			$mun = $dsc;
	}

	if($mun){
			$sqlMUN = "where
        				mun.mundescricao ilike '%".utf8_decode($mun)."%'";
	}

	if(!$uf){
			$sqlUF = "";
	}else{
			$sqlUF = "and
        				mun.estuf like '%".strtoupper(utf8_decode($uf))."%'";
	}

	$sql = "select
				mun.muncod,
				mun.mundescricao,
				mun.estuf
		from
            territorios.municipio mun
        $sqlMUN
        $sqlUF
        group by
			estuf,
			muncod,
			mundescricao
		order by
			estuf";

        $municipios = $db->carregar($sql);

        if(!$municipios){
        	echo "Nenhum resultado encontrado.";
        	return false;
        }

        foreach($municipios as $mun){
        	$estados[] = $mun['estuf'];
        }
        $estados = array_unique($estados);

        $numUF = count($estados);
        $numUF = ($numUF / 3) ;
        $numUF = (int)$numUF;

        echo "<div style=\"max-height:150px;overflow:auto;\" >";

        echo "<div style=\"float:left;width:32%\">";
        $totalUF = 0;

        foreach($estados as $uf){
        		echo "<fieldset><legend>".$uf."</legend>";
        	foreach($municipios as $mun){
        		if($uf == $mun['estuf']){
        			echo "<span style=\"cursor:pointer\" onclick=\"filtraMuncod('{$mun['muncod']}')\" >".$mun['mundescricao']."</span><br>";
        		}
        	}
        	echo "</fieldset>";
        	if($totalUF == $numUF){
        			echo "</div><div style=\"float:left;width:32%\">";
        			$numUF = $numUF * 2;
        	}
        	$totalUF++;
        }
        echo "</fieldset></div>";
	exit;
}


// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

// atribuindo as permissões
$permissoes = verificaPerfilPainel();
// construindo o menu
$enderecosweb = array("/painel/painel.php?modulo=principal/lista&acao=A"     => true,
					  "/painel/painel.php?modulo=principal/lista&acao=A&1=1" => true);

$menu[0] = array("descricao" => "Lista de Indicadores", "link"=> ($enderecosweb[$_SERVER['REQUEST_URI']])?$_SERVER['REQUEST_URI']:key($enderecosweb));
if(!$db->testa_superuser()) {
	$menu[1] = array("descricao" => "Meus indicadores", "link"=> "/painel/painel.php?modulo=principal/lista&acao=A&evento=M");
    if(testaPermissaoTela(null,6190)) $menu[2] = array("descricao" => "Relatório de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
    if(testaPermissaoTela(null,14675)) $menu[3] = array("descricao" => "Relatório de Viagem PR", "link"=> "/painel/painel.php?modulo=principal/relatorioViagemPR&acao=A");
    if(testaPermissaoTela(null,14676)) $menu[4] = array("descricao" => "Relatório Pronatec", "link"=> "/painel/painel.php?modulo=principal/relatorioPronatec&acao=A");
}else{
	$menu[1] = array("descricao" => "Cadastro de Indicadores", "link"=> "/painel/painel.php?modulo=principal/cadastro&acao=A&indid=novoIndicador");
	if(testaPermissaoTela()){
		$menu[2] = array("descricao" => "Relatório de Indicadores", "link"=> "/painel/painel.php?modulo=principal/tabela&acao=A");
		$menu[3] = array("descricao" => "Relatório de Viagem PR", "link"=> "/painel/painel.php?modulo=principal/relatorioViagemPR&acao=A");
		$menu[4] = array("descricao" => "Relatório Pronatec", "link"=> "/painel/painel.php?modulo=principal/relatorioPronatec&acao=A");
	}
}


echo montarAbasArray($menu, $_SERVER['REQUEST_URI']);
// titulos da tela
$titulo = "Relatório de Indicadores";
monta_titulo( $titulo, '&nbsp;' );
?>
<script language="javascript" type="text/javascript" src="/includes/agrupador.js"></script>
<script language="javascript" type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="/includes/funcoes.js"></script>
<form action="" method="post" name="formulario" id="formulario">
<table class="tabela notprint" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
    <td align='right' width=25% class="SubTituloDireita">Município:</td>
    <td>
    	<?=campo_texto('busca_municipio','N','S',30,30,'','','','','','','id="busca_municipio"','BuscaMunicipioEnter2(event);') ?>
    	 <input type="button" value="Buscar" onclick="BuscaMunicipio2();">
    </td>
</tr>
<tr>
	<td colspan=2>
		<div id="resultado_pesquisa">
		</div>
	</td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Estado:</td>
    <td>
    	<?php 
    	$sql = "select estuf from territorios.estado order by estuf "; 
    	$arrEstuf = $db->carregar($sql);
    	echo "<div>";
    	foreach($arrEstuf as $estuf){
    		echo "<div style=\"border: 1px solid black; margin: 1px; padding: 3px; float: left; text-align: center; cursor: pointer;".($_GET['estuf'] == $estuf['estuf'] ? "font-weight:bold;" : "")."\"><a href=\"javascript:filtraEstado('{$estuf['estuf']}')\" >{$estuf['estuf']}</a> </div>";
    	}
    	echo "</div>";
    	?>
    </td>
</tr>
<tr>
    <td align='right' class="SubTituloDireita">Região:</td>
    <td>
    	<?php 
    	$sql = "select regdescricao,regcod from territorios.regiao order by regdescricao "; 
    	$arrEstuf = $db->carregar($sql);
    	echo "<div>";
    	foreach($arrEstuf as $estuf){
    		echo "<div style=\"border: 1px solid black; margin: 1px; padding: 3px; float: left; text-align: center; cursor: pointer;".(trim($_GET['regcod']) == trim($estuf['regcod']) ? "font-weight:bold;" : "")."\" ><a href=\"javascript:filtraRegiao('{$estuf['regcod']}')\" >{$estuf['regdescricao']}</a></div> ";
    	}
    	echo "<div style=\"border: 1px solid black; margin: 1px; padding: 3px; float: left; text-align: center; cursor: pointer;\"> <a href=\"javascript:exibeBrasil()\" >Brasil</a></div></div>";
    	?>
    </td>
</tr>
<?php 
	if ( $_POST['prtid'] && $_POST['carregar'] == 1 ) {
		$arrIndicador = $dados;
	} else {
		$sql = "select 
					indid
				from
					painel.indicador
				where
					indrelatorio is true
				and
					indstatus = 'A'
				and
					indpublicado IS TRUE";
		$arrIndicadores = $db->carregar($sql);
		$arrIndicadores = !$arrIndicadores ? array() : $arrIndicadores;
		foreach($arrIndicadores as $indid){
			$arrIndicador[] = $indid['indid'];
		}
		$arrIndicador = !$arrIndicador ? array(1) : $arrIndicador;
	}
		
		$stSql = " SELECT 
						ind.indid as codigo,
						ind.indid || ' - ' || ind.indnome || ' - ' || aca.acadsc as descricao
					FROM 
						painel.indicador ind
					INNER JOIN
						painel.acao aca ON aca.acaid = ind.acaid
					WHERE
						ind.indstatus = 'A'
					AND
						ind.indpublicado IS TRUE
					AND 
						ind.indrelatorio is true
					ORDER BY
						ind.indnome";
		$stSqlCompleto = " SELECT 
					ind.indid as codigo,
					ind.indid || ' - ' || ind.indnome || ' - ' || aca.acadsc as descricao
				FROM 
					painel.indicador ind
				INNER JOIN
					painel.acao aca ON aca.acaid = ind.acaid
				WHERE
					ind.indid IN (".implode(",",$arrIndicador).")
				AND
					ind.indstatus = 'A'
				AND
					ind.indpublicado IS TRUE
				AND
					ind.indrelatorio is true
				ORDER BY
					ind.indnome";
		mostrarComboPopup( "Indicadores" , "indicador" ,  $stSql, $stSqlCompleto, 'Selecione o(s) Indicadores(s)' );
		
	?>
	<tr>
		<td class="SubTituloDireita">Descrição</td>
		<td>
			<?= campo_texto( 'titulo_indicadores', 'N', 'S', '', 75, 60, '', '', 'left', '', 0, 'id="titulo_indicadores"'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Exibir Observações</td>
		<td>
			<input type=checkbox  id=chkExibirObservacoes value="exibir" checked>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Exibir Responsáveis</td>
		<td>
			<input type=checkbox  id=chkExibirResponsaveis value="exibir" checked>
		</td>
	</tr>
<?php if($db->testa_superuser()){?>
<tr bgcolor="#DCDCDC">
    <td align='center' colspan=2 >
    	<input type="button" name="resetar_arquivos" value="Limpar cache dos relatórios" onclick="resetarArquivos()" />
    	<input type="button" name="salvar_filtro_indicadores" value="Salvar Filtros Indicadores" onclick="salvarFiltros()" />
    </td>
</tr>
<?php } ?>
</table>
<input type="hidden" name="publico" id="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
<input type="hidden" name="prtid" id="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
<input type="hidden" name="carregar" id="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
<input type="hidden" name="excluir" id="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
</form>
<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
		<tr>
			<td onclick="javascript:onOffBloco( 'outros' );">
				<img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
				Relatórios Gerenciais
				<input type="hidden" id="outros_flag" name="outros_flag" value="0" />
			</td>
		</tr>
	</table>	
	<div id="outros_div_filtros_off">
	</div>
	<div id="outros_div_filtros_on" style="display:none;">
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
				<tr>
					<td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
					<td>
					<?php
						$sql = sprintf("
									Select	Case
											  When prtpublico = true and usucpf = '%s' 
												Then '<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
												Else '<img border=\"0\" src=\"../imagens/usuario_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
													  <img border=\"0\" src=\"../imagens/excluir_01.gif\" title=\" Sem ação \">'  
											End as acao, 
											'<b>' || prtdsc || '</b>' as descricao,
											'<b>' || prtobservacao || '</b>' as observacao
									From public.parametros_tela 
									Where mnuid = %d and prtpublico = TRUE
								",
								$_SESSION['usucpf'],
								$_SESSION['mnuid'],
								$_SESSION['usucpf']);
						
						$cabecalho = array('Ação', 'Nome', 'Observações');
						$db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null );
					?>
					</td>
				</tr>
		</table>
	</div>
<!-- MINHAS CONSULTAS -->		
<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
	<tr>
		<td onclick="javascript:onOffBloco( 'minhasconsultas' );" >
			<!-- -->  
			<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
			Minhas Consultas
			<input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />					
		</td>
	</tr>
</table>		
<div id="minhasconsultas_div_filtros_off">
</div>
<div id="minhasconsultas_div_filtros_on" style="display:none;">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td width="195" class="SubTituloDireita" valign="top">Consultas</td>
				<?php
					
					$sql = sprintf(
						"SELECT 
							CASE WHEN prtpublico = false THEN '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
															   <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
															   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
														 ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
														 	   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
							END as acao, 
							'' || prtdsc || '' as descricao 
						 FROM 
						 	public.parametros_tela 
						 WHERE 
						 	mnuid = %d AND usucpf = '%s'",
						$_SESSION['mnuid'],
						$_SESSION['usucpf']
					);
					
					$cabecalho = array('Ação', 'Nome');
				
				?>
				<td>
					<?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, 'N', '80%', null ); ?>
				</td>
			</tr>
	</table>
</div>		
<!-- FIM MINHAS CONSULTAS -->
<script>
function tornar_publico( prtid ){
	document.getElementById('publico').value = '1';
	document.getElementById('prtid').value = prtid;
	document.getElementById('formulario').action = 'painel.php?modulo=principal/tabela&acao=A';
	document.getElementById('formulario').target = '_self';
	document.getElementById('formulario').submit();
}

function excluir_relatorio( prtid ){
	if(confirm( 'Tem certeja que deseja excluir o registro?' )){
		document.getElementById('excluir').value = '1';
		document.getElementById('prtid').value = prtid;
		document.getElementById('formulario').action = 'painel.php?modulo=principal/tabela&acao=A';
		document.getElementById('formulario').target = '_self';
		document.getElementById('formulario').submit();
	}
}

function carregar_relatorio( prtid ){
	document.getElementById('carregar').value = '1';
	document.getElementById('prtid').value = prtid;
	document.getElementById('formulario').action = 'painel.php?modulo=principal/tabela&acao=A';
	document.getElementById('formulario').target = '_self';
	document.getElementById('formulario').submit();
}

function salvarFiltros(){
	if ( document.getElementById('titulo_indicadores').value == '' ) {
		alert( 'É necessário informar a descrição do relatório!' );
		document.getElementById('titulo_indicadores').focus();
		return;
	}
	var nomesExistentes = new Array();
	<?php
		$sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
		$nomesExistentes = $db->carregar( $sqlNomesConsulta );
		if ( $nomesExistentes ){
			foreach ( $nomesExistentes as $linhaNome )
			{
				print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
			}
		}
	?>
	var confirma = true;
	var i, j = nomesExistentes.length;
	for ( i = 0; i < j; i++ ){
		if ( nomesExistentes[i] == document.getElementById('titulo_indicadores').value ){
			confirma = confirm( 'Deseja alterar a consulta já existente?' );
			break;
		}
	}
	if ( !confirma ){
		return;
	}
	selectAllOptions( document.getElementById('indicador') );
	document.getElementById('formulario').action = 'painel.php?modulo=principal/tabela&acao=A&requisicao=salvarIndicadores';
	document.getElementById('formulario').target = '_self';
	document.getElementById('formulario').submit();
}

function BuscaMunicipioEnter2(e)
{
    if (e.keyCode == 13)
    {
        BuscaMunicipio2();
    }
}

<?php if($db->testa_superuser()){?>
function resetarArquivos(){
	if(confirm("Deseja realmente eliminar o cache dos relatórios existentes?")){
		window.location.href="painel.php?modulo=principal/tabela&acao=A&resetar=1";
	}	
}
<?php } ?>

function BuscaMunicipio2(){
	var mun = document.getElementById('busca_municipio');

	if(!mun.value){
		alert('Digite o Município para busca.');
		return false;
	}

	if(mun.value){
		document.getElementById('resultado_pesquisa').innerHTML = "<center>Carregando...</center>";
		var req = new Ajax.Request('painel.php?modulo=principal/tabela&acao=A', {
						        method:     'post',
						        parameters: 'buscaMunicipioAjax=' + mun.value,
						        onComplete: function (res)
						        {
									document.getElementById('resultado_pesquisa').innerHTML = res.responseText;
						        }
						  });

	}
}

/**
	 * Alterar visibilidade de um bloco.	 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}


function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function filtraMuncod(muncod){
	if(document.getElementById('indicador') && document.getElementById('indicador_campo_flag').value == 1){
		selectAllOptions( document.getElementById('indicador') );
		var indicadores = new Array();
		for(i = 0; i < document.getElementById('indicador').options.length;i++){
			indicadores[i] = document.getElementById('indicador').options[i].value;
		}
	}else{
		var indicadores = "todos";
	}
	
	var exibirResponsaveis = document.getElementById('chkExibirResponsaveis').checked;
	var exibirObservacoes = document.getElementById('chkExibirObservacoes').checked;
	window.open('painel.php?modulo=principal/popupTabelaIndicador&acao=A&exResp='+exibirResponsaveis+'&exOb='+exibirObservacoes+'&muncod=' + muncod + '&indid=' + indicadores,'Indicador','scrollbars=yes,height=700,width=880,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function filtraEstado(estuf){
	if(document.getElementById('indicador') && document.getElementById('indicador_campo_flag').value == 1){
		selectAllOptions( document.getElementById('indicador') );
		var indicadores = new Array();
		for(i = 0; i < document.getElementById('indicador').options.length;i++){
			indicadores[i] = document.getElementById('indicador').options[i].value;
		}
	}else{
		var indicadores = "todos";
	}
	
	var exibirResponsaveis = document.getElementById('chkExibirResponsaveis').checked;
	var exibirObservacoes = document.getElementById('chkExibirObservacoes').checked;
	window.open('painel.php?modulo=principal/popupTabelaIndicador&acao=A&exResp='+exibirResponsaveis+'&exOb='+exibirObservacoes+'&estuf=' + estuf + '&indid=' + indicadores,'Indicador','scrollbars=yes,height=700,width=880,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function filtraRegiao(regcod){
	if(document.getElementById('indicador') && document.getElementById('indicador_campo_flag').value == 1){
		selectAllOptions( document.getElementById('indicador') );
		var indicadores = new Array();
		for(i = 0; i < document.getElementById('indicador').options.length;i++){
			indicadores[i] = document.getElementById('indicador').options[i].value;
		}
	}else{
		var indicadores = "todos";
	}
	var exibirResponsaveis = document.getElementById('chkExibirResponsaveis').checked;
	var exibirObservacoes = document.getElementById('chkExibirObservacoes').checked;
	window.open('painel.php?modulo=principal/popupTabelaIndicador&acao=A&exResp='+exibirResponsaveis+'&exOb='+exibirObservacoes+'&regcod=' + regcod + '&indid=' + indicadores,'Indicador','scrollbars=yes,height=700,width=880,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}
function exibeBrasil(){
	if(document.getElementById('indicador') && document.getElementById('indicador_campo_flag').value == 1){
		selectAllOptions( document.getElementById('indicador') );
		var indicadores = new Array();
		for(i = 0; i < document.getElementById('indicador').options.length;i++){
			indicadores[i] = document.getElementById('indicador').options[i].value;
		}
	}else{
		var indicadores = "todos";
	}
	var exibirResponsaveis = document.getElementById('chkExibirResponsaveis').checked;
	var exibirObservacoes = document.getElementById('chkExibirObservacoes').checked;
	window.open('painel.php?modulo=principal/popupTabelaIndicador&acao=A&exResp='+exibirResponsaveis+'&exOb='+exibirObservacoes+'&indid=' + indicadores,'Indicador','scrollbars=yes,height=700,width=880,status=no,toolbar=no,menubar=no,location=no');
	void(0);
}

</script>