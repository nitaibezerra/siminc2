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

/*----------------- Configurações de Página ---------------------*/ 

if( $_REQUEST['excluirPro'] != '' )
{
    $proid = $_REQUEST['excluirPro'];
    if( ( $proid != '1') && ( $proid != 2 ) )
    {
        $sql  = "DELETE FROM pdeescola.criterioeficacia WHERE proid = '$proid'";
        $db->executar( $sql );
        $sql  = "DELETE FROM pdeescola.problema WHERE proid = '$proid'";
        $db->executar( $sql );
        $db->commit();
        $db->sucesso('principal/sintese_autoavaliacao/problemas_criterios');
        exit();
    }
}



if( $_REQUEST['excluirCri'] != '' )
{
    $creid = $_REQUEST['excluirCri'];
    
    $sql  = "DELETE FROM pdeescola.criterioeficacia WHERE creid = '$creid'";
    $db->executar( $sql );
    $db->executar( $sql );
    $db->commit();
    $db->sucesso('principal/sintese_autoavaliacao/problemas_criterios');
    exit();  
}


header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pdeid 	= $_SESSION["pdeid"];
$entid  = $_SESSION["entid"];
if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
	exit();
}

$cDado = array("instrumento" => 'sintese_autoavaliacao');
$cForm = new formulario($cDado);
$cForm->direcPag('');
//$cForm->direcPag('salva(0)',array("id" => 0));

function salva()
{
    echo("<script>
			alert('Dados Salvos.');
		 </script>");
}
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
<form name=formulario method=post>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
		<div class="dtree" style="overflow: hidden;">
			<p>
				<a href="javascript: d.openAll();">Abrir Todos</a>
				&nbsp;|&nbsp;
				<a href="javascript: d.closeAll();">Fechar Todos</a>
			</p>
			<div id="_arvore"></div>
		</div>
	</td>
</tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" align="center">
    <?php echo $cForm->montarbuttons('');?>
    </table>
</form>

<link rel="StyleSheet" href="/includes/dtree/dtree.css" type="text/css" />
<script type="text/javascript" src="/includes/dtree/dtree.js">window.open('/www/includes/dtree/dtree.js');</script>

<?php

//echo $_SESSION['pdeid'];
$sql = "SELECT * FROM pdeescola.problema where pdeid = '{$_SESSION['pdeid']}' ORDER BY proid ";
       
$problema = $db->carregar( $sql );
$problema = $problema ? $problema : array();

?> 
	<script type="text/javascript">

		d = new dTree('d');
		d.config.folderLinks = true;
		d.config.useIcons = false;
		d.config.useCookies = true; 
		
		<?if($estadoPerm || $boSuperUsuario){ 
			if($boSuperUsuario || $boPerfilPerm){?>
				d.add(0,-1,'<a href="javascript:addproblema();"><img align="absmiddle" src="/imagens/gif_inclui.gif" title="Adicionar Problema"></a> - Problemas');<?
			}else{?>
        		d.add(0,-1,'<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Problema"> - Problemas');<? 
        	}
        }else{?>
        	d.add(0,-1,'<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Problema"> - Problemas');<?
        }?>
        
        <?php #workaround para não confundir id dos problemas na arvore (EMERGENCIA) 
        ?>
        <?php $cont = 3000;?>
        <?php for( $i = 0; $i < count($problema); $i++ ){
        		if($estadoPerm || $boSuperUsuario){
		        	if($boSuperUsuario || $boPerfilPerm){
		            	if( $problema[$i]['profixo'] != 't'  ){
		            		$img = '<a href="javascript:addcriterio('.$problema[$i]['proid'].');"><img align="absmiddle" src="/imagens/gif_inclui.gif" title="Adicionar Critério"></a><a href="#" onclick="return excluirproblema('.$problema[$i]['proid'].');">&nbsp;<img align="absmiddle" src="/imagens/excluir.gif" title="Excluir Problema"></a>';
		            	}else{ 
		            		$img = '<a href="javascript:addcriterio('.$problema[$i]['proid'].');"><img align="absmiddle" src="/imagens/gif_inclui.gif" title="Adicionar Critério"></a>';}
		        	}else{
		        		if( $problema[$i]['profixo'] != 't'  ){
		            		$img = '<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Critério">&nbsp;';
		            	}else{ 
		            		$img = '<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Critério">';}
		        	}
        		}else{
        			if( $problema[$i]['profixo'] != 't'  ){
		            	$img = '<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Critério">&nbsp;';
		            }else{ 
		            	$img = '<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Critério">';}
        		}
            $prodesc =  str_replace(chr(10),"",str_replace(chr(13)," ",addslashes($problema[$i]['prodescricao'])));
            if (strlen($prodesc)>90) $prodescresumida = substr($prodesc,0,90).'...'; else  $prodescresumida = $prodesc;
            $desc = '&nbsp;&nbsp;'.($i + 1).' - '.$prodescresumida.'';
            ?>d.add(<?= ($i +1)?>,0,'<?=$img?> <b title="<?=$prodesc?>"><?=$prodescresumida?></b>','','','','/includes/dtree/img/folder.gif','');<?
        
            $sql = "SELECT distinct
                        ac.aceid, 
                        ac.aceidpai,
                        ac.acecodigo,
                        ac.acedescricao,
                        c.creid
                    FROM pdeescola.criterioeficacia as c
                    INNER JOIN pdeescola.analisecriterioeficacia AS ac ON c.aceid = ac.aceid
                    INNER JOIN pdeescola.problema AS p ON p.proid = c.proid
                    WHERE p.proid = '{$problema[$i]['proid']}'
                    AND 
                        ac.aceano = '{$_SESSION['exercicio_atual']}'
                    AND 
                        aceidpai IS NULL
                        ORDER BY ac.aceid";
                     
             $criterio = $db->carregar( $sql );
             
             for( $k = 0; $k < count($criterio); $k++ ){
                 if ( $criterio[$k]['creid'] != '' )
                 {
                  $cont++;
                  $num = (($i + 1).'.'.($k + 1));
                  $img = '';
                   
                  $cridesc =  str_replace(chr(10),"",str_replace(chr(13)," ",addslashes($criterio[$k]['acedescricao'])));
                  if (strlen($cridesc)>90) $cridescresumida = substr($cridesc,0,90).'...'; else  $cridescresumida = $cridesc;
                  	if($estadoPerm || $boSuperUsuario){
                  		if($boSuperUsuario || $boPerfilPerm){
                  			$desc = '<title="Alterar/Excluir Critério"><a href="#" onclick="return excluircriterio('.$criterio[$k]['creid'].');"><img align="absmiddle" src="/imagens/excluir.gif" title="'.$cridescresumida.'"></a>&nbsp;&nbsp;'.$num.' - '.$cridesc.'</a>';
                  		}else{
                  			$desc = '<title="Alterar/Excluir Critério">&nbsp;&nbsp;'.$num.' - '.$cridesc.'</a>';
                  		}
                  	}else{
                  		$desc = '<title="Alterar/Excluir Critério">&nbsp;&nbsp;'.$num.' - '.$cridesc.'</a>';
                  	}
                  ?>d.add( <?=$cont?>, <?=$i + 1?>,'<?=$img?> <b><?=$desc?></b>','','','','/includes/dtree/img/folder.gif','');<?
                 }
             } 
                    
        }   
		?>
		elemento = document.getElementById( '_arvore' );
        elemento.innerHTML = d;
 
	</script>


<script>

//var name = prompt("Please enter your name:","Stephen");
//if (!name) name = 'Hey You!';

//pdeescola.php?modulo=principal/sintese_autoavaliacao/problemas_criterios&acao=A

function addproblema()
{
    return windowOpen('pdeescola.php?modulo=principal/sintese_autoavaliacao/popupProblemaCriterio&acao=I&tipo=problema',
                      'popupProblemaCriterio',
                      'height=240,width=750,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function addcriterio(id )
{
	 return windowOpen('pdeescola.php?modulo=principal/sintese_autoavaliacao/popupProblemaCriterio&acao=I&tipo=criterio&proid='+id,
                      'popupProblemaCriterio',
                      'height=400,width=750,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function excluircriterio(id )
{
     if (!confirm('Deseja excluir o registro?')) 
     {
        return false; 
     }
     else
     {
      	window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/problemas_criterios&acao=A&excluirCri=' + id;	
     }
     
}

function excluirproblema(id)
{
     if (!confirm('Deseja excluir o registro?')) 
     {
     	return false; 
     }
     else
     {
     	return window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/problemas_criterios&acao=A&excluirPro=' + id;
     }
}

function addatividade(id)
{
	janela('geral/popup_problemaprog.php?tipo=3&idpai='+id, 500,180);
}

function editproblema(id)
{
	janela('geral/popup_problemaprog.php?tipo=1&id='+id, 500,180);
}

function editcriterio(id)
{
	janela('geral/popup_problemaprog.php?tipo=2&id='+id, 500,180);
}

function editatividade(id)
{
	janela('geral/popup_problemaprog.php?tipo=3,&id='+id, 500,180);
}

</script>

