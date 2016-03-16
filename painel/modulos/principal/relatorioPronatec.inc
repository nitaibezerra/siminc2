<?
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
$titulo = "Relatório Pronatec";
monta_titulo( $titulo, '&nbsp;' );
?>

<script language="javascript" type="text/javascript" src="/includes/agrupador.js"></script>
<script language="javascript" type="text/javascript" src="/includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="/includes/funcoes.js"></script>
<script>
	function BuscaMunicipioEnter2(e){
		if (e.keyCode == 13){
			BuscaMunicipio2();
		}
	}
	function BuscaMunicipio2(){
		var mun = document.getElementById('busca_municipio');
		
		if(!mun.value){
			alert('Digite o Município para busca.');
			return false;
		}
		if(mun.value){
			document.getElementById('resultado_pesquisa').innerHTML = "<center>Carregando...</center>";
			var req = new Ajax.Request('painel.php?modulo=principal/relatorioPronatec&acao=A', {
									method:     'post',
									parameters: 'buscaMunicipioAjax=' + mun.value,
									onComplete: function (res)
									{
										document.getElementById('resultado_pesquisa').innerHTML = res.responseText;
									}
							  });
		}
	}
	function filtraMuncod(muncod){
        var codper = document.getElementById('codper').value;
		window.open('painel.php?modulo=principal/popuprelatorioPronatec&acao=A&muncod=' + muncod + '&codper=' + codper,'Relatório de Viagem PR','scrollbars=yes,height=700,width=880,status=no,toolbar=no,menubar=no,location=no');
		void(0);
	}
	function filtraEstado(estuf){
        var codper = document.getElementById('codper').value;
		window.open('painel.php?modulo=principal/popuprelatorioPronatec&acao=A&estuf=' + estuf + '&codper=' + codper,'Relatório de Viagem PR','scrollbars=yes,height=700,width=880,status=no,toolbar=no,menubar=no,location=no');
		void(0);
	}
	function filtraRegiao(regcod){
        var codper = document.getElementById('codper').value;
		window.open('painel.php?modulo=principal/popuprelatorioPronatec&acao=A&regcod=' + regcod + '&codper=' + codper,'Relatório de Viagem PR','scrollbars=yes,height=700,width=880,status=no,toolbar=no,menubar=no,location=no');
		void(0);
	}
	function exibeBrasil(){
        var codper = document.getElementById('codper').value;
		window.open('painel.php?modulo=principal/popuprelatorioPronatec&acao=A&codper=' + codper,'Relatório de Viagem PR','scrollbars=yes,height=700,width=880,status=no,toolbar=no,menubar=no,location=no');
		void(0);
	}
</script>
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
		<td colspan="2">
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
    <tr>
        <td align='right' class="SubTituloDireita">Período:</td>
        <?php
        $sql= "SELECT 1 AS codigo, 'Total' AS descricao
                UNION ALL
                SELECT 2 AS codigo, '2011 - 2014' AS descricao
                UNION ALL
                SELECT 3 AS codigo, '2015 - 2018' AS descricao
		";
        ?>
        <td><?=$db->monta_combo('codper',$sql,'S','','','','','','N',"codper","",2);?></td>
    </tr>
</table>
</form>

