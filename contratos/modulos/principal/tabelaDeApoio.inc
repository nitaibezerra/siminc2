<?php
if ($_REQUEST['alterar']) {
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT
                *
            FROM 
                evento.reuniaocomite 
            WHERE
                rcoid = {$_REQUEST['alterar']}";               
    
    $rsDados = $db->carregar( $sql );
}

 if ($_REQUEST['excluir']){    
        $sql = "SELECT 
                    ev.rcoid 
                FROM 
                    evento.reuniaocomite AS r
                LEFT JOIN
                    evento.evento AS ev
                        ON r.rcoid = ev.rcoid
                WHERE 
                    ev.rcoid = {$_REQUEST['excluir']}";
              
         $numRegistros = $db->carregar($sql);   
         $num = count($numRegistros);
         
         if ($num > 1 ){                         
             echo("<script>alert('Nao é possivel a exclusao desta Reunião, pois a mesma possui um ou mais vínculos');\n</script>");
             $var = 1;   
         }
         else { 
          
             $sql = "DELETE FROM evento.reuniaocomite WHERE rcoid = {$_GET['excluir']}";
            
             $db->executar( $sql );
             $db->commit();
             
             header('Location: evento.php?modulo=principal/tabelaDeApoio&acao=A');
          }
          if ($var == 1){              
             echo '<script type="text/javascript">window.location.href = " evento.php?modulo=principal/tabelaDeApoio&acao=A";</script>';
          }           
     }

if ($_POST['action'] == 'salva'){
    if($_REQUEST['rcodescricao']!=''){    //faz o insert ou update
       	$arrDatamI = explode( "/", $_REQUEST['rcodata']);
    	$formataDatamI = $arrDatamI[2].'-'.$arrDatamI[1].'-'.$arrDatamI[0]; 
         if($_REQUEST['alterar'])
         {
                 $sql = "UPDATE 
                            evento.re1uniaocomite 
                         SET 
                         	rcodata		 = '{$formataDatamI}',
                            rcodescricao = '{$_REQUEST['rcodescricao']}' 
                         WHERE 
                            rcoid = '{$_REQUEST['alterar']}'";
                 
                            $db->carregar($sql);
                            $db->commit();
                            
                            header('Location: evento.php?modulo=principal/tabelaDeApoio&acao=A');
        }
        $rcodescricao = strtoupper($_REQUEST['rcodescricao']); 
        //verifica se ja existe uma unidade com a mesma descriçao e tipo
        $sql = "SELECT * FROM 
                    evento.reuniaocomite 
                WHERE 
                    UPPER(rcodescricao) = '$rcodescricao' 
                ";
                    
        $arrReg = $db->carregar($sql);  
        $numReg = count($arrReg[0]);
         
        if( $arrReg && $numReg > 0 ){     
            echo("<script type=\"text/javascript\">alert('Esta Reunião já está cadastrada!');</script>");
        }
        else{        
            $sql = "INSERT INTO 
                          evento.reuniaocomite (rcodata,
                          						rcodescricao
                          						) 
                    VALUES ( 
                    		  '{$formataDatamI}',
                              '{$_REQUEST['rcodescricao']}' 
                            )";
                            
                                                                     
            $db->executar($sql);
            $db->commit(); 
        } 
        
    }
}
include APPRAIZ . 'includes/cabecalho.inc';
?>
   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<br>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tr><td width="100%" align="center" class="TituloTela">Cadastro de Reuniões do Comitê</td></tr><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td></tr></table>
  <form name="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         
            <tr>
                <td class ="SubTituloDireita" align"right"> Reunião do Comitê: </td>
                <td> 
                    <?php
                    $rcodescricao = $rsDados[0]["rcodescricao"];
                    ?>
                     <?= campo_texto('rcodescricao', 'N', $somenteLeitura, '', 100, 600, '', '', 'left', '',  0, 'id="rcodescricao" onblur="MouseBlur(this);"' ); ?>
                </td>
            </tr>
            <tr>
	            <td class ="SubTituloDireita"> Data da Reunião: </td>
	            <td> 
	                	<?php
	                	$rcodata = $rsDados[0]["rcodata"];
	                    ?>
	                    <?= campo_data( 'rcodata','S', 'S', '', 'S' ); ?> 
	            </td>            
            </tr>
            <tr>
            	<td class ="SubTituloDireita"></td>
            	<td align="left">
                     <input type="button" name="btnBuscar" value="Buscar" onclick="validaForm('busca');""/>
					 <input type="hidden" name="action" id="action" value="0">
                     <input type="button" name="btnGravar" value="Salvar" onclick="validaForm('salva');" />
                </td>
            </tr>
          </form>
          <tr>
            <td class="SubTituloDireita" colspan="2">
                <?php
                
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
                    $sql = "SELECT
                                    '<img onclick=\"return alterarReuniaoComite('|| rcoid ||')\" src=\"/imagens/alterar.gif\" title=\"Alterar Registro\" /> <img onclick=\"return excluirReuniaoComite('|| rcoid ||')\" src=\"/imagens/excluir.gif\" title=\"Excluir Registro\" />',
                                    to_char(rcodata::date,'DD/MM/YYYY') AS rcodata,
                                    rcodescricao
                                FROM 
                                     evento.reuniaocomite";  
                }
                $cabecalho = array( "Opções", "Data","Reunião do cômite" );
                   
             	$db->monta_lista($sql, $cabecalho, 20, 10, 'N', '', '' );                
                ?>
            </td>
           </tr>        
       </table>
<script type="text/javascript">
function validaForm( action ){
	var act = document.getElementById('action');
 	act.value = action;
	document.formulario.submit(); 
} 
function excluirReuniaoComite(rcoid)
         {
             if (confirm('Deseja excluir o registro?')) 
             {
                 return window.location.href = 'evento.php?modulo=principal/tabelaDeApoio&acao=A&excluir=' + rcoid;
                
             }
             
         }
         
function alterarReuniaoComite(rcoid)
{    
    return window.location.href = 'evento.php?modulo=principal/tabelaDeApoio&acao=A&alterar=' + rcoid;
}
 
</script>