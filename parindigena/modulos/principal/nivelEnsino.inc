<?php
// arquivo "nivelEnsino.inc"
include APPRAIZ . 'includes/Agrupador.php';
//$db->executar("SET CLIENT_ENCODING TO 'ISO-8859-1'");

//aqui dentro o código para inserçao, alteraçao e exclusao


if ($_REQUEST['alterar']) {
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT
                *
            FROM 
                parindigena.nivelensino 
            WHERE
                nivid = {$_REQUEST['alterar']}";               
    
    $rsDadosNivel = $db->carregar( $sql );
}

 if ($_REQUEST['excluir'])
     {    
       //verificando a integridade para poder excluir o nivel de ensino  
          
        $sql = "SELECT 
                    n.nivid 
                FROM 
                    parindigena.nivelensino AS n
                LEFT JOIN
                    parindigena.materialdidatico AS pm
                        ON n.nivid = pm.nivid
                WHERE 
                    n.nivid = {$_REQUEST['excluir']}";
              
         $numRegistros = $db->carregar($sql);       
                  
         $num = count($numRegistros);
         
         //dbg($num);
         //die();
         //se o count do array do resultado for maior que 0, entao nao sera possivel a exclusao!.
         if ($num > 1 ) 
         {                         
             echo("<script>alert('Nao é possivel a exclusao deste Nível de Ensino, pois o mesmo possui um ou mais vínculos');\n</script>");
             $var = 1;   
         }
         else
         { 
          
             $sql = "DELETE FROM parindigena.nivelensino WHERE nivid = {$_GET['excluir']}";
            
             $db->executar( $sql );
             $db->commit();
             
             header('Location: parindigena.php?modulo=principal/nivelEnsino&acao=I');
          }
          if ($var == 1)
          {              
             echo '<script type="text/javascript">window.location.href = "parindigena.php?modulo=principal/nivelEnsino&acao=I";</script>';
          }
           
     }

 if (array_key_exists('btnGravar', $_REQUEST))
{
    if($_REQUEST['nivdescricao']!='')
    {    //faz o insert ou update
        
         if($_REQUEST['alterar'])
         {
                 $sql = "UPDATE 
                            parindigena.nivelensino 
                         SET 
                            nivdescricao = '{$_REQUEST['nivdescricao']}' 
                         WHERE 
                            nivid = '{$_REQUEST['alterar']}'";
                 
                            $db->carregar($sql);
                            $db->commit();
                            
                            header('Location: parindigena.php?modulo=principal/nivelEnsino&acao=I');
        }
        $nivdescricao = strtoupper($_REQUEST['nivdescricao']); 
        //verifica se ja existe uma unidade com a mesma descriçao e tipo
        $sql = "SELECT * FROM 
                    parindigena.nivelensino 
                WHERE 
                    UPPER(nivdescricao) = '$nivdescricao' 
                ";
                    
        $arrReg = $db->carregar($sql);
        

        $numReg = count($arrReg[0]);
        
       
        //se o numero de registros encontrados for maior do que zero
        if( $arrReg && $numReg > 0 )
        {
     
            echo("<script type=\"text/javascript\">alert('Este Nível de Ensino já está cadastrado!');</script>");
            
 
        }
        else
        {                 
         
            $sql = "INSERT INTO 
                          parindigena.nivelensino (nivdescricao) 
                    VALUES ( 
                              '{$_REQUEST['nivdescricao']}' 
                            )";
                            
                                                                     
            $db->executar($sql);
            $db->commit();
            
             alert('Dados gravados com sucesso!');
                            
        }
             //dbg($sql);
             //die();

          //  header('Location: parindigena.php?modulo=principal/nivelEnsino&acao=I');
        
        }
   }

     


include APPRAIZ . 'includes/cabecalho.inc';
?>
<br>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Nível de Ensino</label></td></tr><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td></tr></table>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadNivelEnsino">
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         
            <tr>
                <td class ="SubTituloDireita" align"right"> Nível de Ensino: </td>
                <td> 
                     <?php
                    $nivdescricao = $rsDadosNivel[0]["nivdescricao"];
                     ?>
                     <?= campo_texto('nivdescricao', 'N', $somenteLeitura, '', 100, 600, '', '', 'left', '',  0, 'id="nivdescricao" onblur="MouseBlur(this);"' ); ?>
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
                                    '<img onclick=\"return alterarNivelEnsino('|| nivid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirNivelEnsino('|| nivid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    nivid, 
                                    nivdescricao
                                FROM 
                                     parindigena.nivelensino";
                                     
                     if( strtoupper($_REQUEST['nivdescricao']))
                     {     
                     $sql.=" WHERE 
                                    UPPER(nivdescricao)  ILIKE '%" .strtoupper($_REQUEST['nivdescricao']). "%' ";
                     }
                   
                     if( strtoupper($_REQUEST['nivdescricao']))
                     {     
                     $sql.=" ORDER BY
                                    nivdescricao ";
                     }               
                                    
                     $buscar = 1;
                }
                else
                {
                    $sql = "SELECT
                                    '<img onclick=\"return alterarNivelEnsino('|| nivid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirNivelEnsino('|| nivid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    nivid,
                                    nivdescricao
                                FROM 
                                     parindigena.nivelensino";  
                }
                $cabecalho = array( "Opções", "Código ", "Nível de Ensino" );
                
                
                
        
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
    var total = cadNivelEnsino.elements.length;
    i = 0;
    while (i <= total)
     {
         if(cadNivelEnsino.elements[i].value == "")
         {
           alert("É necessário o preenchimento de todos os campos");
           
           cadNivelEnsino.elements[i].focus();
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
 function excluirNivelEnsino(nivid)
         {
             if (confirm('Deseja excluir o registro?')) 
             {
                 return window.location.href = 'parindigena.php?modulo=principal/nivelEnsino&acao=I&excluir=' + nivid;
                
             }
             
         }
         
function alterarNivelEnsino(nivid)
{    
    return window.location.href = 'parindigena.php?modulo=principal/nivelEnsino&acao=I&alterar=' + nivid;
}
 
 </script>



