<?php
if ($_REQUEST['alterar']) {
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT 
					            i.iteitem || ' ' as codigo, 
					            pai.iteespecificacao||' -- '||i.iteespecificacao ||' -- '||g.grudescricao as descricao, 
					            umedescricao as unidade,
					            iq.iqtqtdmax as quantidade,
					            cp.coeid
					        FROM evento.item i
					        JOIN evento.unidademedida uni ON uni.umeid = i.umeid
					        JOIN evento.itemquantidade iq ON iq.iteid = i.iteid
					        JOIN evento.grupo g ON g.gruid = iq.gruid
					        JOIN evento.contratopregao cp ON cp.coeid = iq.coeid
					        LEFT JOIN(
					                SELECT itv.iteid, itv.coeid, sum(itc.itcquantidade) AS quantidade_utilizada 
					                FROM evento.itemvalor AS itv
					                JOIN evento.itemconsumo AS itc ON itc.itvid = itv.itvid
					                GROUP BY itv.iteid, itv.coeid
					        ) AS iv ON iv.iteid = i.iteid AND iv.coeid = iq.coeid
					        JOIN(
					                SELECT iteid, iteidpai, iteespecificacao FROM evento.item
					        ) as pai on pai.iteid = i.iteidpai
					        WHERE 1 = 1 
					        AND iq.iqtid = {$_REQUEST['alterar']}";               
    
    $rsDados = $db->pegaLinha( $sql );
}


if ($_POST['action'] == 'salva'){
    
		if($_REQUEST['alterar']){
                $sql = "
                    UPDATE evento.itemquantidade 
                        SET iqtqtdmax = {$_POST['quantidade']}
                    WHERE iqtid = {$_REQUEST['alterar']}
                ";
                $db->carregar($sql);
                $db->commit();
                            
                echo '<script>
                		alert("Operação efetuada com sucesso!");
                		location.href="evento.php?modulo=principal/manutencaoItem&acao=A&coeid='.$_POST['coeid'].'";
                	  </script>';
        }
}

//habilita campos
$somenteLeitura = 'S';
if($rsDados['codigo']) $somenteLeitura = 'N';

include APPRAIZ . 'includes/cabecalho.inc';
?>
   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<br>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Manutenção Item/Quantidade</label></td></tr><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td></tr></table>
  <form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
            	<td align="center" colspan="3" style="BACKGROUND-COLOR: #dcdcdc;">
                    <label class="TituloTela" style="color:#000000;">CONTRATO</label>
                </td>
            </tr>
            <tr>
                <td colspan="3" align="CENTER"> 
                    <?php
                    if($_REQUEST["coeid"]) $_POST["coeid"] = $_REQUEST["coeid"];
                    $coeid = $_POST["coeid"] ? $_POST["coeid"] : $rsDados["coeid"];
                    
                    $sql = " SELECT
                                cp.coeid AS codigo, 
                                cp.coeid ||' - '|| cp.coerazaosocial AS descricao
                           FROM 
                                evento.contratopregao AS cp
                           WHERE cp.coestatus = 'A'
                           ORDER BY 2 ";
                    
                    $db->monta_combo( 'coeid', $sql, $somenteLeitura, '-- Selecione o contrato --','filtraContrato', '', '', '600' );
                    ?>
                </td>
            </tr>
            <tr>
            	<td align="center" colspan="3" style="BACKGROUND-COLOR: #dcdcdc;">
            		<label class="TituloTela" style="color:#000000;">DADOS DO ITEM</label>
            	</td>
            </tr>
            <tr>
                <td width="25%" class ="SubTituloDireita" align"right"> Código: </td>
                <td> 
                    <?php
                    $codigo = $rsDados["codigo"];
                    ?>
                     <?= campo_texto('codigo', 'N', $somenteLeitura, '', 10, 10, '', '', 'left', '',  0, 'id="codigo" onblur="MouseBlur(this);"' ); ?>
                </td>
                <td align="left">
                     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align"right"> Descrição: </td>
                <td> 
                    <?php
                    $descricao = $rsDados["descricao"];
                    ?>
                     <?= campo_texto('descricao', 'N', $somenteLeitura, '', 100, 600, '', '', 'left', '',  0, 'id="descricao" onblur="MouseBlur(this);"' ); ?>
                </td>
                <td align="left">
                     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align"right"> Unidade: </td>
                <td> 
                    <?php
                    $unidade = $rsDados["unidade"];
                    ?>
                     <?= campo_texto('unidade', 'N', $somenteLeitura, '', 100, 600, '', '', 'left', '',  0, 'id="unidade" onblur="MouseBlur(this);"' ); ?>
                </td>
                <td align="left">
                     
                </td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align"right"> Quantidade: </td>
                <td> 
                    <?php
                    $quantidade = $rsDados["quantidade"];
                    ?>
                     <?= campo_texto('quantidade', 'N', 'S', '', 10, 6, '', '', 'left', '',  0, 'id="quantidade" onblur="MouseBlur(this);"' ); ?>
                </td>
                <td align="left">
                     
                </td>
            </tr>
            <tr>
            	<td align="center" colspan="3" style="BACKGROUND-COLOR: #dcdcdc;">
                     <input type="button" name="btnGravar" value="Salvar" onclick="validaForm('salva');" />
                     <input type="hidden" name="action" id="action" value="0">
                     <?if($somenteLeitura == 'S'){?>
                     <!-- 
                     	<input type="button" name="btnBuscar" value="Buscar" onclick="validaForm('busca');""/>
                      -->
                     <?}?>
                </td>
            </tr>
          </form>
          <tr>
            <td class="SubTituloDireita" colspan = "3">
                <?php
                
                /*
                if ($_POST['action'] == 'busca'){
                    $sql = "SELECT
                                    '<img onclick=\"return alterarReuniaoComite('|| rcoid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirReuniaoComite('|| rcoid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    rcoid, 
                                    to_char(rcodata::date,'DD/MM/YYYY') AS rcodata,
                                    rcodescricao
                                FROM 
                                     evento.reuniaocomite
                                WHERE rcodescricao IS NOT NULL";
    
                     if( strtoupper($_REQUEST['rcodescricao'])){     
	                     $sql.=" AND 
	                                    UPPER(rcodescricao)  ILIKE '%" .strtoupper($_REQUEST['rcodescricao']). "%' ";
                     }                   
                     if( strtoupper($_REQUEST['rcodescricao'])){     
	                     $sql.=" ORDER BY
	                             rcodescricao ";
                     }               
                                    
                     $buscar = 1;
                }
                else {
				
                    $sql = "
                        SELECT  '<img onclick=\"return alterarReuniaoComite('|| rcoid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" />
                                <img onclick=\"return excluirReuniaoComite('|| rcoid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                to_char(rcodata::date,'DD/MM/YYYY') AS form_rcodata,
                                rcodescricao
                                
                        FROM evento.reuniaocomite
                        ORDER BY rcodata DESC
                    "; 
                
                }
				*/
                
                if($coeid && $somenteLeitura == 'S'){
	                $sql = "SELECT 
	                			'<img onclick=\"return alterarReuniaoComite('|| iq.iqtid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" />' as acao,
					            i.iteitem || ' ' as codigo, 
					            pai.iteespecificacao||' -- '||i.iteespecificacao ||' -- '||g.grudescricao as descricao, 
					            umedescricao as unidade,
					            iq.iqtqtdmax as quantidade
					        FROM evento.item i
					        JOIN evento.unidademedida uni ON uni.umeid = i.umeid
					        JOIN evento.itemquantidade iq ON iq.iteid = i.iteid
					        JOIN evento.grupo g ON g.gruid = iq.gruid
					        JOIN evento.contratopregao cp ON cp.coeid = iq.coeid
					        LEFT JOIN(
					                SELECT itv.iteid, itv.coeid, sum(itc.itcquantidade) AS quantidade_utilizada 
					                FROM evento.itemvalor AS itv
					                JOIN evento.itemconsumo AS itc ON itc.itvid = itv.itvid
					                GROUP BY itv.iteid, itv.coeid
					        ) AS iv ON iv.iteid = i.iteid AND iv.coeid = iq.coeid
					        JOIN(
					                SELECT iteid, iteidpai, iteespecificacao FROM evento.item
					        ) as pai on pai.iteid = i.iteidpai
					        WHERE 1 = 1 
					        and cp.coeid = $coeid
					        GROUP BY  
					                iq.iqtid,
					                i.iteitem, 
					                g.grudescricao, 
					                i.iteespecificacao, 
					                iqtqtdmax, 
					                pai.iteespecificacao, 
					                umedescricao,
					                iv.quantidade_utilizada
					        ORDER BY 
					                coalesce(split_part(i.iteitem, '.', 1),'0')::numeric, 
					                coalesce(split_part(i.iteitem, '.', 2),'0')::numeric, 
					                case when split_part(i.iteitem, '.', 3)='' then '0' else split_part(i.iteitem, '.', 3) end::numeric";
				                
	                $cabecalho = array( "Ação", "Código", "Descrição", "Unidade", "Quantidade" );
	                   
	             	$db->monta_lista($sql, $cabecalho, 50, 10, 'N', '', '' );    
                }            
                ?>
            </td>
           </tr>        
       </table>
<script type="text/javascript">

function filtraContrato()
{
	var act = document.getElementById('action');
 	act.value = '';
 	document.formulario.action = "evento.php?modulo=principal/manutencaoItem&acao=A";
	document.formulario.submit();
}

function validaForm( action ){

	var alterar = "<?=$_REQUEST['alterar']?>";
	if(document.formulario.coeid.value == ''){
		alert('Selecione o contrato');
		return false;
	}
	if(alterar == ''){
		alert('Selecione o item para alterar a quantidade');
		return false;
	}


	var act = document.getElementById('action');
 	act.value = action;
	document.formulario.submit(); 
} 

         
function alterarReuniaoComite(rcoid)
{    
    return window.location.href = 'evento.php?modulo=principal/manutencaoItem&acao=A&alterar=' + rcoid;
}
 
</script>