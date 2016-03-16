<?php
// arquivo "areaTematica.inc"
include APPRAIZ . 'includes/Agrupador.php';
//$db->executar("SET CLIENT_ENCODING TO 'ISO-8859-1'");

//aqui dentro o código para inserçao, alteraçao e exclusao


if ($_REQUEST['alterar']) {
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT
                *
            FROM 
                parindigena.areatematica 
            WHERE
                areid = {$_REQUEST['alterar']}";               
    
    $rsDadosArea = $db->carregar( $sql );
}

 if ($_REQUEST['excluir'])
     {    
       //verificando a integridade para poder excluir o area tematica   
         
        $sql = "SELECT 
                    at.areid 
                FROM 
                    parindigena.areatematica AS at
                LEFT JOIN
                    parindigena.materialdidatico AS pm
                        ON at.areid = pm.areid
                WHERE 
                    at.areid = {$_REQUEST['excluir']}";
              
         $numRegistros = $db->carregar($sql);       
                  
         $num = count($numRegistros);
         
         //dbg($num);
         //die();
         //se o count do array do resultado for maior que 0, entao nao sera possivel a exclusao!.
         if ($num > 1 ) 
         {                         
             echo("<script>alert('Nao é possivel a exclusao desta Área Temática, pois a mesma possui um ou mais vínculos');\n</script>");
             $var = 1;   
         }
         else
         { 
         
             $sql = "DELETE FROM parindigena.areatematica WHERE areid = {$_GET['excluir']}";
            
             $db->executar( $sql );
             $db->commit();
             
             header('Location: parindigena.php?modulo=principal/areaTematica&acao=I');
          }
          if ($var == 1)
          {              
             echo '<script type="text/javascript">window.location.href = "parindigena.php?modulo=principal/areaTematica&acao=I";</script>';
          }
           
     }

 if (array_key_exists('btnGravar', $_REQUEST))
{
    if($_REQUEST['aredescricao']!='')
    {    //faz o insert ou update
        
         if($_REQUEST['alterar'])
         {
                 $sql = "UPDATE 
                            parindigena.areatematica 
                         SET 
                            aredescricao = '{$_REQUEST['aredescricao']}' 
                         WHERE 
                            areid = '{$_REQUEST['alterar']}'";
                 
                            $db->carregar($sql);
                            $db->commit();
                            
                            header('Location: parindigena.php?modulo=principal/areaTematica&acao=I');
        }
        $aredescricao = strtoupper($_REQUEST['aredescricao']); 
        //verifica se ja existe uma unidade com a mesma descriçao e tipo
        $sql = "SELECT * FROM 
                    parindigena.areatematica 
                WHERE 
                    UPPER(aredescricao) = '$aredescricao' 
                ";
                    
        $arrReg = $db->carregar($sql);
        

        $numReg = count($arrReg[0]);
        
        
        //se o numero de registros encontrados for maior do que zero
        if( $arrReg && $numReg > 0 )
        {
            echo("<script type=\"text/javascript\">alert('Esta Area Temática já está cadastrada!');</script>");
        }
        else
        {                 
         
            $sql = "INSERT INTO 
                          parindigena.areatematica (aredescricao) 
                    VALUES ( 
                              '{$_REQUEST['aredescricao']}' 
                              )";
                              
            $db->executar($sql);
            $db->commit();
            
             alert('Dados gravados com sucesso!');
            
        }
             //dbg($sql);
             //die();
                                         

           // header('Location: parindigena.php?modulo=principal/areaTematica&acao=I');

        }
   }

     


include APPRAIZ . 'includes/cabecalho.inc';
?>
<br>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Área Temática</label></td></tr><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td></tr></table>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadAreaTematica">
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         
            <tr>
                <td class ="SubTituloDireita" align"right"> Área Temática: </td>
                <td> 
                     <?php
                    $aredescricao = $rsDadosArea[0]["aredescricao"];
                     ?>
                     <?= campo_texto('aredescricao', 'N', $somenteLeitura, '', 100, 600, '', '', 'left', '',  0, 'id="aredescricao" onblur="MouseBlur(this);"' ); ?>
                </td>
                <td align="left">
                     <input type="submit" name="btnBuscar" value="Buscar" />
                     <input type="submit" name="btnGravar" value="Salvar" />
                </td>
            </tr>
          
          </form>
          <tr>
            <td class="SubTituloDireita" colspan = "3">
                <?php
                
                if (array_key_exists('btnBuscar', $_REQUEST))
                {
                    $sql = "SELECT
                                    '<img onclick=\"return alterarAreaTematica('|| areid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirAreaTematica('|| areid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    areid, 
                                    aredescricao
                                FROM 
                                     parindigena.areatematica";
                                     
                     if( strtoupper($_REQUEST['aredescricao']))
                     {     
                     $sql.=" WHERE 
                                    UPPER(aredescricao)  ILIKE '%" .strtoupper($_REQUEST['aredescricao']). "%' ";
                     }
                   
                     if( strtoupper($_REQUEST['aredescricao']))
                     {     
                     $sql.=" ORDER BY
                                    aredescricao ";
                     }               
                                    
                     $buscar = 1;
                }
                else
                {
                    $sql = "SELECT
                                    '<img onclick=\"return alterarAreaTematica('|| areid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirAreaTematica('|| areid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    areid,
                                    aredescricao
                                FROM 
                                     parindigena.areatematica";  
                }
                $cabecalho = array( "Opções", "Código ", "Nome da Área Tematica" );
                
                
                
        
              $db->monta_lista($sql, $cabecalho, 20, 10, 'N', '', '' );
                
                ?>
            </td>
           </tr>
        
       </table>
       
 <script type="text/javascript">

/* Funçao para validar os dados do formulario requisitante.
 * author: PedroDantas
 * date: 2008-08-25
 */
function validaForm()
{  
    var total = cadAreaTematica.elements.length;
    i = 0;
    while (i <= total)
     {
         if(cadAreaTematica.elements[i].value == "")
         {
           alert("É necessário o preenchimento de todos os campos");
           
           cadAreaTematica.elements[i].focus();
           return false;
           break;
         }
        i++;
     }   
 }
 
 /* Funçao para excluir uma unidade de medida
 * author: PedroDantas
 * date: 2008-08-25
 * params: undid (id da unidade selecionada)
 */
 function excluirAreaTematica(areid)
         {
             if (confirm('Deseja excluir o registro?')) 
             {
                 return window.location.href = 'parindigena.php?modulo=principal/areaTematica&acao=I&excluir=' + areid;
                
             }
             
         }
         
function alterarAreaTematica(areid)
{    
    return window.location.href = 'parindigena.php?modulo=principal/areaTematica&acao=I&alterar=' + areid;
}
 
 </script>



