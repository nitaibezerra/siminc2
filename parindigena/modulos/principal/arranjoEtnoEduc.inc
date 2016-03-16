<?php
// arquivo "territorioEtnoEduc.inc"
include APPRAIZ . 'includes/Agrupador.php';
//$db->executar("SET CLIENT_ENCODING TO 'ISO-8859-1'");

//aqui dentro o código para inserçao, alteraçao e exclusao


if ($_REQUEST['alterar']) {
 
    $sql = "SELECT
                *
            FROM 
                parindigena.territorioetnoeducacional 
            WHERE
                terid = {$_REQUEST['alterar']}";               
    
    $rsDadosArr = $db->carregar( $sql );
}

 if ($_REQUEST['excluir'])
     {    
  
       
        $sql = "    SELECT 
                        t.terid
                    FROM 
                        parindigena.territorioetnoeducacional AS t
                    LEFT JOIN 
                        parindigena.territorioformacaoinicial AS tfi 
                            ON t.terid = tfi.terid
                    LEFT JOIN 
                        parindigena.territorioformacaocontinuada AS tfc 
                            ON t.terid = tfc.terid
                    LEFT JOIN 
                        parindigena.territoriomaterialdidatico AS tm 
                            ON t.terid = tm.terid
                    LEFT JOIN
                        parindigena.territorioobrainfraestrutura AS ti
                            ON t.terid = ti.terid
                    WHERE
                        t.terid =  {$_REQUEST['excluir']}";
              
         $numRegistros = $db->carregar($sql);       
                  
         $num = count($numRegistros);
         
         //dbg($num);
         //die();
         //se o count do array do resultado for maior que 0, entao nao sera possivel a exclusao!.
         if ($num > 1 ) 
         {                         
             echo("<script>alert('Nao é possivel a exclusao deste Território, pois o mesmo possui um ou mais vínculos');\n</script>");
             $var = 1;   
         }
         else
         { 
          
             $sql = "DELETE FROM parindigena.territorioetnoeducacional WHERE terid = {$_GET['excluir']}";
            
             $db->executar( $sql );
             $db->commit();
             
             header('Location: parindigena.php?modulo=principal/arranjoEtnoEduc&acao=I');
          }
          if ($var == 1)
          {              
             echo '<script type="text/javascript">window.location.href = "parindigena.php?modulo=principal/arranjoEtnoEduc&acao=I";</script>';
          }
           
     }

 if (array_key_exists('btnGravar', $_REQUEST))
{
    if($_REQUEST['ternome']!='')
    {    //faz o insert ou update
        
         if($_REQUEST['alterar'])
         {
                 $sql = "UPDATE 
                            parindigena.territorioetnoeducacional 
                         SET 
                            ternome = '{$_REQUEST['ternome']}' 
                         WHERE 
                            terid = '{$_REQUEST['alterar']}'";
                 
                            $db->carregar($sql);
                            $db->commit();
                            
                            header('Location: parindigena.php?modulo=principal/arranjoEtnoEduc&acao=I');
        }
        $ternome = strtoupper($_REQUEST['ternome']); 
         
        $sql = "SELECT * FROM 
                    parindigena.territorioetnoeducacional 
                WHERE 
                    UPPER(ternome) = '$ternome' 
                ";
                    
        $arrReg = $db->carregar($sql);
        

        $numReg = count($arrReg[0]);
        
       
        //se o numero de registros encontrados for maior do que zero
        if( $arrReg && $numReg > 0 )
        {
     
            echo("<script type=\"text/javascript\">alert('Este Território já está cadastrado!');</script>");
            
           
        }
        else
        {                 
         
            $sql = "INSERT INTO 
                          parindigena.territorioetnoeducacional (ternome) 
                    VALUES ( 
                              '{$_REQUEST['ternome']}' 
                            )";
                            
                                                                     
            $db->executar($sql);
            $db->commit();
            
             alert('Dados gravados com sucesso!');
                            
        }
             //dbg($sql);
             //die();

          //  header('Location: parindigena.php?modulo=principal/territorioEtnoEduc&acao=I');
        
        }
   }

     


include APPRAIZ . 'includes/cabecalho.inc';
?>
<br>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Território Etno-Educacional</label></td></tr><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td></tr></table>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadTerritório">
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         
            <tr>
                <td class ="SubTituloDireita" align"right"> Território Etno-educacional: </td>
                <td> 
                     <?php
                    $ternome = $rsDadosArr[0]["ternome"];
                     ?>
                     <?= campo_texto('ternome', 'N', $somenteLeitura, '', 100, 600, '', '', 'left', '',  0, 'id="ternome" onblur="MouseBlur(this);"' ); ?>
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
                                    '<img onclick=\"return alterarTerritório('|| terid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirTerritório('|| terid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    terid, 
                                    ternome
                                FROM 
                                     parindigena.territorioetnoeducacional";
                                     
                     if( strtoupper($_REQUEST['ternome']))
                     {     
                     $sql.=" WHERE 
                                    UPPER(ternome)  ILIKE '%" .strtoupper($_REQUEST['ternome']). "%' ";
                     }
                   
                     if( strtoupper($_REQUEST['ternome']))
                     {     
                     $sql.=" ORDER BY
                                    ternome ";
                     }               
                                    
                     $buscar = 1;
                }
                else
                {
                    $sql = "SELECT
                                    '<img onclick=\"return alterarTerritório('|| terid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirTerritório('|| terid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    terid,
                                    ternome
                                FROM 
                                     parindigena.territorioetnoeducacional";  
                }
                $cabecalho = array( "Opções", "Código ", "Território Etno-educacional" );
                
                
                
        
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
    var total = cadTerritório.elements.length;
    i = 0;
    while (i <= total)
     {
         if(cadTerritório.elements[i].value == "")
         {
           alert("É necessário o preenchimento de todos os campos");
           
           cadTerritório.elements[i].focus();
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
 function excluirTerritório(terid)
         {
             if (confirm('Deseja excluir o registro?')) 
             {
                 return window.location.href = 'parindigena.php?modulo=principal/arranjoEtnoEduc&acao=I&excluir=' + terid;
                
             }
             
         }
         
function alterarTerritório(terid)
{    
    return window.location.href = 'parindigena.php?modulo=principal/arranjoEtnoEduc&acao=I&alterar=' + terid;
}
 
 </script>



