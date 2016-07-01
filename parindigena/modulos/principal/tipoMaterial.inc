<?php
// arquivo "tipoMaterial.inc"
include APPRAIZ . 'includes/Agrupador.php';
//$db->executar("SET CLIENT_ENCODING TO 'ISO-8859-1'");

//aqui dentro o código para inserçao, alteraçao e exclusao


if ($_REQUEST['alterar']) {
    // query para carregar o laboratorio selecionado.
    $sql = "SELECT
                *
            FROM 
                parindigena.tipomaterial 
            WHERE
                tipid = {$_REQUEST['alterar']}";               
    
    $rsDadosMaterial = $db->carregar( $sql );
}

 if ($_REQUEST['excluir'])
     {    
       //verificando a integridade para poder excluir o povo indigena   
        
        $sql = "SELECT 
                    t.tipid 
                FROM 
                    parindigena.tipomaterial AS t
                LEFT JOIN
                    parindigena.materialdidatico AS pm
                        ON t.tipid = pm.tipid
                WHERE 
                    t.tipid = {$_REQUEST['excluir']}";
              
         $numRegistros = $db->carregar($sql);       
                  
         $num = count($numRegistros);
         
         //dbg($num);
         //die();
         //se o count do array do resultado for maior que 0, entao nao sera possivel a exclusao!.
         if ($num > 1 ) 
         {                         
             echo("<script>alert('Nao é possivel a exclusao deste Tipo de Material, pois o mesmo possui um ou mais vínculos');\n</script>");
             $var = 1;   
         }
         else
         { 
          
             $sql = "DELETE FROM parindigena.tipomaterial WHERE tipid = {$_GET['excluir']}";
            
             $db->executar( $sql );
             $db->commit();
             
             header('Location: parindigena.php?modulo=principal/tipoMaterial&acao=I');
          }
          if ($var == 1)
          {              
              echo '<script type="text/javascript">window.location.href = "parindigena.php?modulo=principal/tipoMaterial&acao=I";</script>';
          }
           
     }

 if (array_key_exists('btnGravar', $_REQUEST))
{
    if($_REQUEST['tipdescricao']!='')
    {    //faz o insert ou update
        
         if($_REQUEST['alterar'])
         {
                 $sql = "UPDATE 
                            parindigena.tipomaterial 
                         SET 
                            tipdescricao = '{$_REQUEST['tipdescricao']}' 
                         WHERE 
                            tipid = '{$_REQUEST['alterar']}'";
                 
                            $db->carregar($sql);
                            $db->commit();
                            
                            header('Location: parindigena.php?modulo=principal/tipoMaterial&acao=I');
         }
        $tipdescricao = strtoupper($_REQUEST['tipdescricao']); 
        //verifica se ja existe uma unidade com a mesma descriçao e tipo
        $sql = "SELECT * FROM 
                    parindigena.tipomaterial 
                WHERE 
                    UPPER(tipdescricao) = '$tipdescricao' 
                ";
                    
        $arrReg = $db->carregar($sql);
        

        $numReg = count($arrReg[0]);
        
        
        //se o numero de registros encontrados for maior do que zero
        if( $arrReg && $numReg > 0 )
        {
            echo("<script type=\"text/javascript\">alert('Este Tipo de Material já está cadastrado!');</script>");
        }
        else
        {                 
         
            $sql = "INSERT INTO 
                          parindigena.tipomaterial (tipdescricao) 
                    VALUES ( 
                              '{$_REQUEST['tipdescricao']}' 
                            )";
                            
            $db->executar($sql);
            $db->commit();
            
             alert('Dados gravados com sucesso!');
            
        }
             //dbg($sql);
             //die();

           // header('Location: parindigena.php?modulo=principal/tipoMaterial&acao=I');
                            
       
        }
   }

     


include APPRAIZ . 'includes/cabecalho.inc';
?>
<br>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;"><tr><td width="100%" align="center"><label class="TituloTela" style="color:#000000;">Tipo de Material</label></td></tr><tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr='#FFFFFF', endColorStr='#dcdcdc', gradientType='1')" ></td></tr></table>
    <script type="text/javascript" src="/includes/prototype.js"></script>
    <form onSubmit="return validaForm(this);" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="cadTipoMaterial">
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         
            <tr>
                <td class ="SubTituloDireita" align"right"> Tipo de Material: </td>
                <td> 
                     <?php
                    $tipdescricao = $rsDadosMaterial[0]["tipdescricao"];
                     ?>
                     <?= campo_texto('tipdescricao', 'N', $somenteLeitura, '', 100, 600, '', '', 'left', '',  0, 'id="tipdescricao" onblur="MouseBlur(this);"' ); ?>
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
                                    '<img onclick=\"return alterarTipoMaterial('|| tipid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirTipoMaterial('|| tipid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    tipid, 
                                    tipdescricao
                                FROM 
                                     parindigena.tipomaterial";
                                     
                     if( strtoupper($_REQUEST['tipdescricao']))
                     {     
                     $sql.=" WHERE 
                                    UPPER(tipdescricao)  ILIKE '%" .strtoupper($_REQUEST['tipdescricao']). "%' ";
                     }
                   
                     if( strtoupper($_REQUEST['tipdescricao']))
                     {     
                     $sql.=" ORDER BY
                                    tipdescricao ";
                     }               
                                    
                     $buscar = 1;
                }
                else
                {
                    $sql = "SELECT
                                    '<img onclick=\"return alterarTipoMaterial('|| tipid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" /><img onclick=\"return excluirTipoMaterial('|| tipid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />',
                                    tipid,
                                    tipdescricao
                                FROM 
                                     parindigena.tipomaterial";  
                }
                $cabecalho = array( "Opções", "Código ", "Tipo de Material" );
                
                
                
        
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
    var total = cadTipoMaterial.elements.length;
    i = 0;
    while (i <= total)
     {
         if(cadTipoMaterial.elements[i].value == "")
         {
           alert("É necessário o preenchimento de todos os campos");
           
           cadTipoMaterial.elements[i].focus();
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
 function excluirTipoMaterial(tipid)
         {
             if (confirm('Deseja excluir o registro?')) 
             {
                 return window.location.href = 'parindigena.php?modulo=principal/tipoMaterial&acao=I&excluir=' + tipid;
                
             }
             
         }
         
function alterarTipoMaterial(tipid)
{    
    return window.location.href = 'parindigena.php?modulo=principal/tipoMaterial&acao=I&alterar=' + tipid;
}
 
 </script>
