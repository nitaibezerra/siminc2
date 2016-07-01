<head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title>Cadastro de Obras</title>
	<script type="text/javascript" src="../includes/funcoes.js"></script>
	<script src="js/parindigena.js" type="text/javascript"></script>
	<script src="js/obras.js" type="text/javascript"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>    
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	
    <style type="text/css" media="screen">
        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
            position: absolute;
            width: 100%;
            text-align: center;
            z-index: 8000;
            height: 100%;
        }

        #loader {
            background-color: #fff;
            color: #000033;
            width: 300px;
            border: 2px solid #cccccc;
            font-size: 12px;
            padding: 25px;
            font-weight: bold;
            margin: 150px auto;
        }
        
    </style>
  </head>


<?php

// Inclusão de arquivos padrão do SIMEC
require_once APPRAIZ . 'includes/Agrupador.php';

// Inclusão de arquivos do componente de Entidade 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual   = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual   = substr($caminho_atual, 0 , $posicao_caminho);

$covid = $_REQUEST["covid"];
//$obrid  = $_REQUEST["obrid"];

//dbg($_REQUEST);
//die;

// Executa as funções da tela de acordo com suas ações
switch ($_REQUEST["requisicao"]){
	case "cadastro":
		// Cadastra a obra
		obras_cadastra_obras($_REQUEST);
	break;
	case "atualiza":
		// Atualiza a obra
		obras_atualiza_obras($_REQUEST);
	break;
	default:
	break;	
}

echo "<br/>";

unset($_SESSION["obrid"]);

// Se tiver a sessão, está em atualização de obra
if($_REQUEST["obrid"] || $_SESSION["obrid"]){
	
	if(!$_SESSION["obrid"]){
		$_SESSION["obrid"] = $_REQUEST["obrid"];
		$campus = '';
	}else{
		if($_REQUEST["obrid"]){
			unset($_SESSION["obrid"]);
			$_SESSION["obrid"] = $_REQUEST["obrid"];
		}
	}
	
	$obrid = $_SESSION["obrid"];
	$dados_obra = obras_busca_obras($obrid);
	$requisicao = "atualiza";
	
	// Cria as abas do módulo
	$db->cria_aba($abacod_tela,$url,$parametros);
	
}else{
	$requisicao = "cadastro";
	$campus = 'display:none;';
}

// Cria o título da tela
$titulo_modulo = "Dados da Obra";
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");

?>
<script type="text/javascript">
	
	function enviaDados(id){
		
		//alert(id);
		
		if (id==-1){
			if (Validacao())
				document.formulario.submit();
		}
		else{
			document.formulario.requisicao.value = "";
			document.formulario.salvar.enabled = false;
			document.formulario.voltar.enabled = false;
			//alert(12);
			document.formulario.submit();
			
		}
	}
	
	function buscaObrassss(id){
		
		var link = window.location.href;
		link = link + '&covid=' + id;
		window.location.href = link;
		
		/*
		return;
		var exc = new Ajax.Request(window.location.href,
									{
									   method: 'post',
									   parameters: 'acao=A&grpid=4&estuf=AL&covid=' + id;
									   onComplete: function(r)
									   {
										   alert('blz foi');
									   }
									}
								  );
		*/
	}
</script>

<!-- INÍCIO DO FORMULÁRIO -->
<!--form id="formulario" name="formulario" method="post" onSubmit="return Validacao();" action="<?php echo $caminho_atual . "acao=A" ?>"-->
<form id="formulario" name="formulario" method="post" action="<?php echo $caminho_atual . "acao=A" ?>">
	<table class="tabela listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
            <td colspan="2">
            	<span style="text-align: left;display: block;font-size:100%;margin-bottom:5px;color:blue"><?php echo $_REQUEST['estuf']; ?></span>
            </td>
        </tr>
        <tr>
            <td>                                
            	<span style="display: block;font-size:100%;color:blue"><img src="../imagens/seta_filho.gif">Obras Indígenas</span>
            </td>
        </tr>
		<tr>
			<td class="subtitulodireita">Escolha o Convênio:</td>
			<td align="left" >
                <?php    
					if($_SESSION["obrid"]){
					
						$covid = $db->pegaLinha("SELECT 
						                            pc.covid AS codigo, 
						                            fc.covnumero AS descricao
						                        FROM
						                            parindigena.convenioindigena AS pc
						                        INNER JOIN
						                            financeiro.convenio AS fc ON fc.covid = pc.covid
						                        INNER JOIN 
													parindigena.itemmonitoramento item ON fc.covid = item.covid
						                        WHERE 
						                            item.itmstatus = 'A'
													AND item.estuf = '{$_REQUEST["estuf"]}'
													AND item.obrid = {$_SESSION["obrid"]}
						                            AND pc.coitipo = 'O'", 0);
					
						$covid = $covid['codigo'];
	                }
	                
	                
                	$sql = "SELECT
                				c.covid as codigo,
                				c.covnumero as descricao
                			FROM 
                				obras.conveniosobra c
                			WHERE 
                				c.covestuf = '{$_REQUEST["estuf"]}' AND
                				c.covtipo = 'C'
                			ORDER BY
                				descricao";
                	
	                $db->monta_combo('covid', $sql, 'S', "Selecione...", 'javascript:enviaDados', '', '', '200', 'S', $covid);
                ?>
			</td>
		</tr>
		
		<tr>
			<td class="subtitulodireita">Escolha a Obra:</td>
			<td align="left" >
                <?php
                	
                	$sql = "SELECT 
								oi.obrid AS codigo,
								oi.obrdesc AS descricao
							FROM 
								obras.obrainfraestrutura oi
							INNER JOIN 
								obras.formarepasserecursos fr ON fr.obrid = oi.obrid
							INNER JOIN
								obras.conveniosobra oc ON oc.covid = fr.covid
							INNER JOIN
								entidade.endereco ee ON oi.endid = ee.endid
							WHERE
								oi.cloid = 4 AND 
								ee.estuf = '".$_REQUEST["estuf"]."' AND 
								oc.covid = " . ( $_REQUEST["covid"] ? $_REQUEST["covid"] : 'null' );
                			
	                $db->monta_combo('obrid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', $_REQUEST['obrid']);
                ?>
			</td>
		</tr>
		
		<tr>
			<td style="font-weight: bold" colspan="2">PAR Indígena</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Povos Atendidos:</td>
			<td>
				<?php 
	
					if($_SESSION["obrid"]){
						
						$povid = $db->carregar("SELECT
									p.povid as codigo,
									p.povnome as descricao 
								FROM
									parindigena.povoindigena p
								INNER JOIN
									parindigena.povoobrainfraestrutura pp ON pp.povid = p.povid
								WHERE
									pp.obrid = {$_SESSION["obrid"]}
								ORDER BY povnome");						
					}
					$sql = "
						SELECT
							povid AS codigo,
							povnome AS descricao
						FROM
							parindigena.povoindigena
						ORDER BY povnome";	
						//echo $sql;
					combo_popup( "povid", $sql, "Selecione o(s) Povo(s) Indígena(s)", "192x400", 0, array(), "", "S", false, false, 8, 310 );
	 				
				?>
			</td>
		</tr>
		
		<tr>
        	<td class="subtitulodireita">Línguas:</td>
            <td colspan="5" align=left >
            <?php
                				
                if($_SESSION["obrid"]){
                	$linid = $db->carregar("SELECT 
		                                  lf.linid as codigo,
		                                  l.linnome as descricao
		                             FROM 
		                                  parindigena.linguaobrainfraestrutura AS lf
		                             INNER JOIN parindigena.linguaindigena l ON lf.linid = l.linid
		                             WHERE 
		                                  lf.obrid = {$_SESSION["obrid"]}");                			
                }
                
                $sql = "SELECT 
                           linid as codigo, 
                           linnome as descricao
                        FROM 
                            parindigena.linguaindigena
                        ORDER BY linnome";
                //echo $sql;
                combo_popup( "linid", $sql, "Selecione a(s) Língua(s) Indígena(s)", "192x400", 0, array(), "", "S", false, false, 5, 310 );
            ?>
        	<img src="../imagens/obrig.gif">
        	</td>
        </tr>
		
		<tr>
			<td class="SubTituloDireita">Territórios Atendidos:</td>
			<td>
				<?php 
					
					if($_SESSION["obrid"]){
						$teoid = $db->carregar("SELECT
											t.terid as codigo,
											t.ternome as descricao 
										FROM
											parindigena.territorioetnoeducacional t
										INNER JOIN
											parindigena.territorioobrainfraestrutura tt ON tt.terid = t.terid
										WHERE
											tt.obrid = {$_SESSION["obrid"]}
										ORDER BY t.ternome");
					}
					$sql = "
						SELECT
							terid AS codigo,
							ternome AS descricao
						FROM
							parindigena.territorioetnoeducacional
						ORDER BY ternome";
						//echo $sql;
					combo_popup( "teoid", $sql, "Selecione o(s) Território(s) Indígena(s)", "192x400", 0, array(), "", "S", false, false, 8, 310 );
	 				
				?>
			</td>
		</tr>		
		
		<tr bgcolor="#C0C0C0">
			<td></td>
			<td>
				<div style="float: left;">
					<input type="hidden" name="requisicao" 	value="<?php echo $requisicao; ?>" />
					<input type="hidden" name="estuf" 		value="<?=$_REQUEST["estuf"]?>" />
					<input type="hidden" name="itmid" 		value="<?=$_REQUEST["itmid"]?>" />
					<input type="button" name="salvar"		value="Salvar" style="cursor: pointer" onclick="enviaDados(-1)" <?php if($somenteLeitura=="N") echo "disabled"; ?>> 
					<input type="button" name="voltar"		value="Voltar" style="cursor: pointer" onclick="history.back(-1);">
				</div>
			</td>
		</tr>
	</table>
</form>
