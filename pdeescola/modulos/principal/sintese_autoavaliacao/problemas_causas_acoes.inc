<?php
# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();

/*----- Recuperando $docid para usar na função pegarEstadoAtual ------*/ 
$docid  = pegarDocid( $_SESSION['entid'] );
$estado = pegarEstadoAtual( $docid );
$estadoPerm = (($estado == 76) || ($estado == 37));

/*----------------- Verificando Perfil ---------------------*/ 
$perfis = arrayPerfil();
$boSuperUsuario = in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis );
$boPerfilPerm = ((in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis)) || (in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis)));

/*--------------------------------------*/ 
$proid = (integer) $proid === 0 || $proid === '' || !$proid ? $_REQUEST['alterar'] : $proid;

/*----------------- Configurações de Página ---------------------*/ 
if( ($_REQUEST['alterar']) || ($proid ))
{
    //carrega os dados
 
    $sql = "SELECT 
                *
            FROM 
                pdeescola.problema AS p
            LEFT JOIN 
                pdeescola.principalacao AS pa ON p.proid = pa.proid";
    if(   $_REQUEST['profixo'] == 't'  )
    {
    $sql .= " LEFT JOIN
                pdeescola.causaprovavel AS ca ON p.proid = ca.capproblema
            WHERE 
            p.proid = '{$_REQUEST['alterar']}'
                ";
    }
    else
    {
    $sql .= " LEFT JOIN
                pdeescola.problemacausaprovavel AS ca ON p.proid = ca.proid
            WHERE 
            p.proid = '{$_REQUEST['alterar']}'
                ";
    }
    
    $sql .= " AND p.pdeid = '{$_SESSION['pdeid']}'";

    $rsDadosProblema = $db->carregar($sql);    
    //header('Location: pdeescola.php?modulo=principal/sintese_autoavaliacao/problemas_causas_acoes&acao=A');
}


//insere as causas dos prolemas 1 e 2   
function insereCausa( $proid, $descricao, $capidee )
{    
    global $db;
 
        $sql = "INSERT INTO 
                    pdeescola.problemacausaprovavel
                    (
                    proid,
                    capid,
                    prcdescricao 
                    )
                VALUES
                    (
                    '$proid',
                    '$capidee',
                    '$descricao'
                    )
                    ";
     
        $db->executar( $sql );
} 

 
function insereCausaFixa( $proid, $descricao )
{	 
    global $db;
     
    $proid   = $_REQUEST['alterar'];
    $profixo = $_REQUEST['profixo'];
     
        $sql = "INSERT INTO 
                    pdeescola.problemacausaprovavel
                    (
                    proid,
                    prcdescricao
                    )
                VALUES
                    (
                    '$proid',
                    '$descricao'
                    )
                    ";
                              
        $db->executar( $sql );
} 

 

function deletaCausa( $proid )
{
     global $db;
    
    $proid = $_REQUEST['alterar'];

    $sql = "DELETE FROM pdeescola.problemacausaprovavel WHERE proid = '$proid'";
 
    if ( !$db->executar($sql))
    {
       return false;
    }

}

$desc = $rsDadosProblema[0]['pradescricao'];
function salva()
{     
    global $db;
    global $desc;   
   
    $proid                  = $_REQUEST['alterar'];
    $pradescricao           = $_REQUEST['pradescricao'];
 
    if( ( $desc != '' ) && ( $_REQUEST['pradescricao'] != ''))
    {
        $sql = "UPDATE  
                    pdeescola.principalacao 
                SET 
                    pradescricao = '$pradescricao'
                where 
                    proid = '$proid'
                     ";
    }
    else
    {

    // deleta registro antes do insert, evitando salvar vários registros
    $del = "DELETE FROM pdeescola.principalacao as p 
    		 WHERE proid = '$proid' 
    	   ";
 	$db->executar( $del );

         $sql = "INSERT
                 INTO
                    pdeescola.principalacao
                        (proid, pradescricao)
                 VALUES
                        ( '$proid', '$pradescricao');";
                                          
    }

    $insert = $db->executar( $sql );
    $profixo = $_REQUEST['profixo'];
    if( $profixo == 't' )
    {  
        deletaCausa( $proid );
        $numCausa = count($_REQUEST['capid']);    
        
        if ($numCausa > 0)
        {                
          
           for ( $i = 0; $i < $numCausa; $i++ )
           {    
                if($_REQUEST['capid'][$i] != '')
                {  
  
                   //deletaCausaTabela( $capidee ); 
                   $sqlPegaNome = "SELECT capid, capdescricao FROM pdeescola.causaprovavel WHERE capid = '{$_REQUEST['capid'][$i]}'"; 
                   
                   
                   $rsPegaNome = $db->carregar( $sqlPegaNome ); 
                   
                   //echo $sqlPegaNome; 
                 
                  
                   
                   $descricao = $rsPegaNome[0]['capdescricao'];
                   $capidee   = $rsPegaNome[0]['capid'];
                   
                   //$capide = $_REQUEST['capid'][$i];
                   insereCausa( $proid, $descricao, $capidee );
                }  
           }
        }
                                        
     }
     else // SE O PROBLEMA FOR DIFERENTE DE 1 OU 2
     {
         if (  $_REQUEST['prcdescricao']) 
         {
             for( $k = 0; $k < count( $_REQUEST['prcid'] ); $k++) 
             {
                $capdescricao = $_REQUEST['prcdescricao'][$k];
                 
                if (($_REQUEST['prcid'][$k] == '' ) && ($capdescricao != '')) 
                {
                    //INSERT
                    insereCausaFixa( $proid, $capdescricao );
					//echo("vai fazer insert");
					//die();
                }
                elseif( $capdescricao != '' )
                {
                    //UPDATE
                    
                   $sql = "UPDATE pdeescola.problemacausaprovavel 
                                SET
                                    prcdescricao = '$capdescricao'
                                WHERE
                                prcid = '{$_REQUEST['prcid'][$k]}'";
                                
                   $update = $db->executar( $sql );
                }
             }
         }
                
     }
     $pdeid = $_SESSION['pdeid'];
     
     
     //IMPLEMENTAR AQUI TODA A VERIFICAÇÃO DE PREENCHIMENTO DOS PROBLEMAS
     
	 $existe_preenchimento = $db->pegaUm("SELECT 
										pprid
									 FROM 
									 	pdeescola.pdepreenchimento
									 WHERE
									 	pdeid = '$pdeid'
										 AND
										 	ppritem = 'problemas_causas_acoes'");
	if( $existe_preenchimento == NULL )
	{
		$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento ,ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'si', 'problemas_causas_acoes','{$_SESSION['exercicio_atual']}')";
		$db->executar( $sql_p );
	}       
    $db->commit();      
}



if (array_key_exists('excluir', $_REQUEST)) 
{
    $sql = "DELETE FROM pdeescola.problemacausaprovavel WHERE prcid = '{$_REQUEST['excluir']}'";
    $deleteCausa = $db->executar( $sql );
    $db->commit();      
    exit();
    
   
}
 

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");




/*
echo("<pre>");
print_r( $_REQUEST );
echo("</pre>");
*/




$pdeid 	= $_SESSION["pdeid"];
$entid  = $_SESSION["entid"];
if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!"\n"Tente novamente.\');
			history.go(-1);
		 </script>');
}

$cDado = array("instrumento" => 'sintese_autoavaliacao');
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Síntese da Autoavaliação ', '');
echo cabecalhoPDE(); 

/*----------------- Recupera dados ---------------------*/  
$dados = $db->pegaLinha("SELECT 	pdeuf,
								pdemunicipio, 
								pdenome,
								pdenomediretor,
								pdebairro,
								pdelogradouro,
								pdecomplemento,
								pdetelefone,
								pdeemail,
								tloid
						FROM pdeescola.pdeescola
						WHERE pdeid = '".$pdeid."'");
extract($dados);
if($dados['tloid'] == 1){
	$check1 = 'checked="checked"';
}else if($dados['tloid'] == 2){
	$check2 = 'checked="checked"';
}else if($dados['tloid'] == 3){
	$check3 = 'checked="checked"';
}

$sql = "SELECT entnome as nome
		FROM entidade.entidade as e
		INNER JOIN entidade.funcaoentidade as fe ON fe.entid = e.entid
		WHERE funid = 3 and
		  	  tpcid IN (1,3) AND
	    	  e.entid IN ('{$entid}')";
$NomeEscola = $db->pegaum($sql);
$pdenome = $NomeEscola;



$sqlVerifica = "SELECT * FROM pdeescola.problema WHERE pdeid = '{$_SESSION['pdeid']}' AND profixo IS NOT NULL";
$verifica = $db->pegaUm( $sqlVerifica );

if( $verifica == '' )
{
    $sql_p1 = "INSERT INTO pdeescola.problema ( pdeid, prodescricao, profixo ) VALUES ('{$_SESSION['pdeid']}', 'Acessibilidade ao prédio escolar inadequada (Programa Escola Acessível).', true)";
    $db->executar( $sql_p1 );
    $sql_p2 = "INSERT INTO pdeescola.problema ( pdeid, prodescricao, profixo ) VALUES ('{$_SESSION['pdeid']}', 'Instalações do laboratório de informática inadequadas (Programa Proinfo).', true)";
    $db->executar( $sql_p2 );
    $db->commit();
}

?>
 

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    <body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<?=subCabecalho(' Problemas X Causas prováveis X Ações '); ?>
    <?php if( $_REQUEST['profixo'] == 't' ){
      // echo("<script>alert('cheufoesijf');\n</script>");
        $processaCombosPopup = "processaCombosPopup();";
    }
    ?>
    <form onSubmit=" <?php echo $processaCombosPopup; ?>return validaForm(this);"action="<?php echo $_SERVER['REQUEST_URI'];?>" method="post" id="formulario" name="formulario" >
            <table  id="itensComposicaoSubAcao" class="tabela listagem" border="0" align="center" cellpadding="0" cellspacing="0" style="text-align: center">
                       
                 <tr>
                    <td class="subtitulodireita"> Problema: </td>
                    <td align="left" >  
                    <?php $prodescricao = $rsDadosProblema[0]["prodescricao"]; ?>
                    <?= campo_texto('prodescricao', 'N', 'N', '', 97, 100, '', '', 'left', '',  0, 'id="prodescricao" onblur="MouseBlur(this);"' ); ?>
                </tr>
                <tr>
                    <td class="subtitulodireita">Causas Prováveis</td>
                    <div id="loader-container">
                         <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div> 
                    </div>
                    <?php
                    if( $_REQUEST['profixo'] != 't') 
                    {//prototype
                        ?>
                        <td align="left">
                         
    
                              <div style="height: 100px; width: 500; background-color: #eee; border: none;" class="div_rolagem">
                                <table id="itensCausasProblemas" name="itensCausasProblemas" border="0" align="center" cellpadding="0" cellspacing="0" width="100%" style="text-align: center">
                                  <colgroup>
                                    <col width="10%" />
                                    <col width="90%" />
                                  </colgroup>
                   
                                  <tr style="text-align: center">
                                    <td style="padding: 2px; font-weight: bold;">&nbsp;</td>
                                    <td style="padding: 2px; font-weight: bold;">Causa Provavel </td>
                                  </tr>
            
                                </table>
                               
                              </div>
                          
                          <div style="padding: 5px 0px 0px 5px"></div>
                        <span style="cursor:pointer" id="linkInserirItem" onclick="return adicionarCausa('','');"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Inserir Causas</span>
                          
                        <?php
                    }
                    else{
                        ?>
                        <td align="left">
                        <?php
                        
                        $sql_capid = "SELECT cp.capid FROM pdeescola.problemacausaprovavel AS cp WHERE cp.prcid = '{$_REQUEST['alterar']}'";
                        $capid_unico = $db->pegaUm ( $sql_capid );
                        
                         
                        
                        /*
                        $sql = "SELECT 
                                    c.capid as codigo, 
                                    cp.prcdescricao as descricao   
                                FROM pdeescola.problemacausaprovavel AS cp  
                                INNER JOIN  pdeescola.causaprovavel AS c ON c.capid = cp.capid
                                ";
                                */
                                $sql = "SELECT c.capid as codigo, 
                                    c.capdescricao as descricao   FROM pdeescola.causaprovavel as c  
                                    where capproblema = '{$_REQUEST['capproblema']}'";

 

                         
                        $sqlCapid = "SELECT 
                                       p.capid as codigo,
                                       p.prcdescricao as descricao  
                                     FROM 
                                       pdeescola.problemacausaprovavel as p
                                     INNER JOIN pdeescola.causaprovavel AS c ON p.capid = c.capid  
                                     WHERE 
                                        p.proid = '{$_REQUEST['alterar']}'";
                                       	//echo $sql;
                                       	//echo("<br>");
                                       	//echo $sqlCapid;
                                       	//die();            
          
                                    if( $_REQUEST['alterar'] != '' )
                                    {
                                        $capid    = $db->carregar($sqlCapid);
                                    }
                        
                        combo_popup( "capid", $sql, "Selecione o(s) causas prováveis", "192x400", 0, array(), "", "S", false, false, 5, 500 );
                    }?>
                    </td>
                </tr>
                
                
                <tr>
                    <td class="subtitulodireita"> Principais Ações: </td>
                    <td align="left" >  
                    <?php $pradescricao = $rsDadosProblema[0]['pradescricao']; ?>
                     <?= campo_textarea( 'pradescricao', 'S', 'S', 'pradescricao', 100 , 10, 500, $funcao = '', $acao = 0, $txtdica = '', $tab = false ); ?>  
                </tr>
                <?php echo $cForm->montarbuttons('validaForm()');?>  
    </table>
</form>   

               <?php
//             
//                $sql = "SELECT
//                            '<img onclick=\"return alterarProblema('|| p.proid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" />' as acoes,
//                            p.prodescricao,
//                            pa.pradescricao
//                        FROM 
//                            pdeescola.problema AS p
//                        LEFT JOIN 
//                            pdeescola.principalacao AS pa ON p.proid = pa.proid
//                        
//                        
//                             ";  
                //<img onclick=\"return excluirProblema('|| p.proid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />
                
                $cabecalho = array( "Editar", "Problema", "Principais Ações");
                //$db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '', '' );         

                ?>
<table style="color: rgb(51, 51, 51);" class="listagem" align="center" border="0" cellpadding="2" cellspacing="0" width="95%">
<thead>
<tr>
    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" align="center" valign="top">
        Editar
    </td>
    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" align="center" valign="top">
        Problema
    </td>
    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" align="center" valign="top">
        Causas Prováveis
    </td>
    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" align="center" valign="top">
        Principais Ações
    </td>
</tr>
</thead>
<tbody>

<?php

  /*
echo("<pre>");
print_r( $_SESSION['pdeid'] );
echo("</pre>");
 */
//die();
 
/*
$sql = "SELECT
            p.proid,
            p.prodescricao,
            pa.pradescricao
        FROM 
            pdeescola.problema AS p
        INNER JOIN 
            pdeescola.principalacao AS pa ON p.proid = pa.proid        
        AND p.pdeid = '{$_SESSION['pdeid']}'";
  */
  
 $sql = " SELECT * FROM pdeescola.problema as p where pdeid = '{$_SESSION['pdeid']}' ORDER BY profixo ,prodescricao";

 
       /*
$sql = "SELECT 
	

FROM 
	pdeescola.problema AS p 

LEFT JOIN pdeescola.principalacao AS pa ON p.proid = pa.proid 
WHERE 
p.proid  < 3";
*/
 

$rs = $db->carregar( $sql );                             


for ( $i = 0; $i < count( $rs ); $i++ )
{
 ?>
    <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';" bgcolor="">
        <td title="Editar">
        <?php
		// 76 = estado atual -->diagnótico ou 37 = estado atual --> aguardando correção(cadastramento)
		if( $estadoPerm || $boSuperUsuario ){ 
			if($boSuperUsuario || $boPerfilPerm){
       			if(( $i == 0 ) || ( $i == 1 ) ){
            		$capproblema = ($i + 1);?>
        			<img onclick="return alterarProblema(<?php echo $rs[$i]['proid']; ?>, '<?php echo $rs[$i]['profixo']; ?>','<?php echo $capproblema; ?>' )" src="/imagens/alterar.gif" title="Alterar Registro" border="0"><?php
				}else{?>
		            <img onclick="return alterarProblema(<?php echo $rs[$i]['proid']; ?>, '<?php echo $rs[$i]['profixo']; ?>', '' )" src="/imagens/alterar.gif" title="Alterar Registro" border="0"><?php 
				}
		   	}else{
		   	    if(( $i == 0 ) || ( $i == 1 ) ){
            		$capproblema = ($i + 1);?>
        			<img src="/imagens/alterar.gif" title="Não é permitido alterar Registro" border="0"><?php
				}else{?>
		            <img src="/imagens/alterar.gif" title="Não é permitido alterar Registro" border="0"><?php 
				}
		   	}	
		}else{
		       if(( $i == 0 ) || ( $i == 1 ) ){
            		$capproblema = ($i + 1);?>
        			<img src="/imagens/alterar.gif" title="Não é permitido alterar o registro" border="0"><?php 
		       }else{?>
		            <img src="/imagens/alterar.gif" title="Não é permitido alterar o registro" border="0"><?php 
		       }
		}?>

            </td>
        <td title="Problema">
            <?php echo $rs[$i]['prodescricao']; ?>
        </td>
        <td title="Causas">
            <?php 
                $sqlCausa = "SELECT  
                               prcdescricao  
                             FROM 
                                pdeescola.problemacausaprovavel
                             WHERE 
                                proid = '{$rs[$i]['proid']}'";
            // echo $sqlCausa;
                $rsCausa = $db->carregar( $sqlCausa );
                
                for ( $j = 0; $j < count( $rsCausa ); $j++ )
                {
                
                    if( $rsCausa[$j]['prcdescricao'] != '' )
                    {
                        echo '<b> * </b>'.$rsCausa[$j]['prcdescricao'].'<br>';
                    }
                    else
                    {
                    ?>
                        <center> --<br><br></center>
                    <?php
                    }
                }
               
                ?>     <br><br>
      
        </td>
        <td title="Principais Ações">
 
        <?php 
        
        $sqlDesc = "select pradescricao from pdeescola.principalacao where proid = '{$rs[$i]['proid']}'";
        $pradescricao = $db->pegaUm( $sqlDesc );
        
        if( $pradescricao != '' )
        {
            echo $pradescricao; 
        }
        else
        {           
        ?>
            <center> -- </center>
        <?php
        }
        ?>
        
            </td>
    </tr>
 <?php
}
?>
             </table>


<script type="text/javascript">
function alterarProblema( proid, profixo, capproblema )
{    
    var profixo;
    var capproblema;
    if(profixo == '')
    {  
    //return window.location.href = 'pdeescola.php?'+"<?php echo $_SERVER['argv'][0]; ?>"+'&alterar='+proid;
    return window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/problemas_causas_acoes&acao=A&alterar='+proid;
    }
    else
    {
    //return window.location.href = 'pdeescola.php?'+"<?php echo $_SERVER['argv'][0]; ?>"+'&alterar='+proid;
    return window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/problemas_causas_acoes&acao=A&alterar='+proid+'&profixo='+profixo+'&capproblema='+capproblema;
    }
}


   /**
     * 
     */
function adicionarCausa(prcid, prcdescricao )
    {

        var input_prcid;        
        var input_prcdescricao;
        
        var btnExcluirProblema = document.createElement('img');
        btnExcluirProblema.src     = '/imagens/excluir.gif';
        btnExcluirProblema.onclick = function()
        {
            removerCausa(this.parentNode.parentNode.getAttribute("id"), prcid);
        }
        
        //NUMERO DA ETAPA

        //Data inicial
        input_prcdescricao    = document.createElement('input');
        input_prcdescricao.setAttribute('type', 'text');
        input_prcdescricao.setAttribute('class', 'normal');
        input_prcdescricao.setAttribute('name', 'prcdescricao[]');
        input_prcdescricao.setAttribute('maxlength', '500');
        input_prcdescricao.setAttribute('size', '80');
        input_prcdescricao.setAttribute('value', prcdescricao);
  
        
        //hidden, id do material
        input_prcid     = document.createElement('input');
        input_prcid.setAttribute('type', 'hidden');
        input_prcid.setAttribute('name', 'prcid[]');
        input_prcid.setAttribute('value', prcid);
        
             
        var tabelaCausasProblemas = $('itensCausasProblemas');
        var novaLinha                    = tabelaCausasProblemas.insertRow(tabelaCausasProblemas.rows.length);
        novaLinha.id                     = 'linhaitensCausasProblemas' + tabelaCausasProblemas.rows.length + 1;
        novaLinha.style.textAlign        = 'center';
        novaLinha.style.backgroundColor  = '#f0f0f0';
 
        /**
         * Botao excluir
         */
        var novaCelula0 = novaLinha.insertCell(novaLinha.cells.length);
        novaCelula0.appendChild(btnExcluirProblema);

        //numero da etapa
        //var novaCelulaNum = novaLinha.insertCell(novaLinha.cells.length);
        //novaCelulaNum.appendChild(input_num);
 
        /**
         * Data inicio
         */
        var novaCelula1 = novaLinha.insertCell(novaLinha.cells.length);
        input_prcdescricao.setAttribute('id', novaLinha.cells.length);
        novaCelula1.appendChild(input_prcdescricao);
        novaCelula1.appendChild(input_prcid);


        return false;
    }

 function carregarCausasProblemas(){
    <?php
        
    if( $_REQUEST['alterar'] )
    {    
        if( ($_REQUEST['alterar'] == '1') || ($_REQUEST['alterar'] =='2'))
        {
            $itens = $db->carregar('SELECT
                                        capid,
                                        capdescricao
                                    FROM
                                        pdeescola.causaprovavel
                                    WHERE
                                        capproblema  = ' . $_REQUEST['alterar'] . '
                                    ');
        }
        else
        {
            $itens = $db->carregar('SELECT
                                        prcid,
                                        prcdescricao
                                    FROM
                                        pdeescola.problemacausaprovavel
                                    WHERE
                                        proid  = ' . $_REQUEST['alterar'] . '
                                    ');
        }
       
    }

    if (!is_array($itens)) {
        echo 'return false;';
    } else {
        while (list(, $item) = each($itens)) {
            $item = array_map('addslashes', $item);

            echo "adicionarCausa('" , $item['prcid'] , "','" , trim($item['prcdescricao']) ,"');";
        }
    }
?>}
<?php if ( $_REQUEST['profixo'] != 't' ) 
{ ?>
    carregarCausasProblemas();
    $('loader-container').hide();
<?php } ?>   
 $('loader-container').hide();
function fecharJanela()
{
    if (_modificado) {
        if (!confirm('Existem dados que não foram salvos.\nDeseja fechar a janela sem salvá-los?'))
            return false;
    }

    window.close();
}


    function removerCausa(linha, prcid)
    { 
        var linha = $(linha);
        linha.style.backgroundColor = 'rgb(255,255,210)';
        linha.style.backgroundColor = '#DAE0D2';

        if (!confirm('Atenção! Os dados serão apagados permanentemente!\nDeseja realmente excluir o item selecionado?')) {
            linha.style.backgroundColor = '#f0f0f0';
        } else {
            $('loader-container').show();
            try {
                if (prcid != '') {
                    //linha.parentNode.removeChild(linha);
                    //window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/problemas_causas_acoes&acao=A&excluir='+capid;
                    
                    var exc = new Ajax.Request(window.location.href,
                                               {
                                                   method: 'post',
                                                   parameters: 'excluir=' + prcid,
                                                   onComplete: function(r)
                                                   {
                                                       //if (r.responseText == 'null')
                                                           linha.parentNode.removeChild(linha);
                                                           //window.location.reload();
                                                       //else
                                                       //    alert('Não foi possível excluir o item.');
                                                           linha.style.backgroundColor = '#f0f0f0';
                                                   }
                                               });
                                         /*     */
                } else {
                    linha.parentNode.removeChild(linha);
                }
            } catch (e) {
            }

            $('loader-container').hide();
        }

        return false;
    }

function processaCombosPopup()
{
// alert('chegou');

    selectAllOptions( document.getElementById( 'capid' ) );

}

function validaForm()
{
    var problema = formulario.prodescricao.value.length;
    var pradescricao = formulario.pradescricao.value.length;
    
    if( problema < 1 )
    {
        alert('Selecione um Problema na tabela abaixo.');
        return false;
    }
    
    /*
    if( pradescricao < 1 )
    {
        alert('O Campo "principal ação" é Obrigatório.');
        return false;
    }
    */
    <?php
    if( $_REQUEST['profixo'] == 't' )
    {?>
        processaCombosPopup();
    <?php
    }
    ?>
        
    
    document.formulario.submit();
    return true;
}
    
</script>
