<?php
# Verfica se o pdeid e intid estão na sessão
#teste
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

if( $_REQUEST['excluir'] != '' )
{
    $id = $_REQUEST['excluir'];
    
    excluiFilhos( $id );
    $sql  = "DELETE FROM pdeescola.planosuporteestrategico WHERE pseid = '$id'";
   
    $db->executar( $sql );
    
    
	$arObjetivo = $db->carregar("SELECT pseid FROM pdeescola.planosuporteestrategico WHERE pdeid = ".$_SESSION["pdeid"]." AND pseidpai is null");
	if(is_array($arObjetivo) && count($arObjetivo)){
		# Navega nos objetivos encontrados
		$boTemMeta = false;
		foreach($arObjetivo as $objetivo){
			# Verifica se existe Estrategias
			$arEstrategia = $db->carregar("SELECT pseid FROM pdeescola.planosuporteestrategico WHERE pdeid = ".$_SESSION["pdeid"]." AND pseidpai = ". $objetivo['pseid']);
			if(is_array($arEstrategia) && count($arEstrategia)){
				# Navega nas Estrategias
				foreach($arEstrategia as $estrategia){
					# Verifica se existe Metas
					$arMeta = $db->carregar("SELECT pseid FROM pdeescola.planosuporteestrategico WHERE pdeid = ".$_SESSION["pdeid"]." AND pseidpai = ". $estrategia['pseid']);
					if(is_array($arMeta) && count($arMeta)){
						$boTemMeta = true;
					}
				}
			}		
		}
	}
	# Se existe não exite metas, limpa tabela preenchimento 
	if(!$boTemMeta){
		$sql = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$_SESSION["pdeid"]." AND ppritem = 'popupObjetivosMetas'" ;
		$db->executar( $sql );
	}
    
    $db->commit();
    $db->sucesso('principal/sintese_autoavaliacao/objetivos_estrategias_metas');
    exit();  
}

function excluiFilhos( $id ){
	
	global $db;
	
    $sql = "SELECT pseid FROM pdeescola.planosuporteestrategico WHERE pseidpai = $id";		
			
	$arPlanos = $db->carregar( $sql );
	
	if( is_array( $arPlanos ) ){
		
		foreach( $arPlanos as $plano ){
			
			excluiFilhos( $plano["pseid"] );
									
			$sql  = "DELETE FROM pdeescola.planosuporteestrategico WHERE pseid = '{$plano["pseid"]}'";
			
			$db->executar( $sql );
		}
	}
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

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Plano de Ação ', '');
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

?>


<?=subCabecalho(' Plano de Suporte Estratégico ');?>

<form name=formulario method=post>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
<tr>
	<td style="padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
		<div class="dtree" style="overflow: hidden;">
			<p>
				<a style="font-size: 11px; color: #333333" href="javascript: d.openAll();">Abrir Todos</a>
				&nbsp;|&nbsp;
				<a style="font-size: 11px; color: #333333" href="javascript: d.closeAll();">Fechar Todos</a>
				&nbsp;|&nbsp;
				<a style="font-size: 11px; color: #333333" href="pdeescola.php?modulo=principal/sintese_autoavaliacao/consultaPlanoDeAcaoReport&acao=A">Vizualizar Todos os Planos</a>
				<?//verifica se ja não tem algo justificado
				$sqlJust = "select op.tprid from pdeescola.justificativa AS ju
							inner join pdeescola.opcaojustificativa AS op on ju.opjid = op.opjid
							where ju.pdeid = '{$_SESSION['pdeid']}'";
	
				$arrJustificados = $db->carregar( $sqlJust );
				?>
				<? if($arrJustificados){?>
				&nbsp;|&nbsp;
				<a onclick="return cadastraJustificativa();" style="font-size: 11px; cursor: hand; cursor:pointer; color: #333333">Visualizar Justificativas</a>
				
				<?} ?> 
				&nbsp;|&nbsp;
				<a style="font-size: 11px; color: #333333" href="pdeescola.php?modulo=principal/sintese_autoavaliacao/reportPaf&acao=A">PAF</a>
				
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
$sql  = "SELECT * FROM pdeescola.planosuporteestrategico WHERE pdeid = '{$_SESSION['pdeid']}' AND pseidpai IS NULL ORDER BY pseid";
  
//echo $_SESSION['pdeid'];
       
$objetivo = $db->carregar( $sql  );
$objetivo = $objetivo ? $objetivo : array();

?> 
	<script type="text/javascript">

		d = new dTree('d');
		d.config.folderLinks = true;
		d.config.useIcons = false;
		d.config.useCookies = true; 
				
		<?if( ($boSuperUsuario || $boPerfilPerm) && ($boSuperUsuario || $estadoPerm) ){?>
				d.add(0,-1,'<a href="javascript:addobjetivo();"><img align="absmiddle" src="/imagens/gif_inclui.gif" title="Adicionar Objetivo"></a> - Objetivos');
          <?}else{?>
      			d.add(0,-1,'<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Objetivo"> - Objetivos');
      	  <?}?>
      	  
        <?php for( $i = 0; $i < count($objetivo); $i++ ){
            
        	$objdesc =  str_replace(chr(10),"",str_replace(chr(13)," ",addslashes($objetivo[$i]['psedescricao'])));
            if (strlen($objdesc)>90) $objdescresumida = substr($objdesc,0,90).'...'; else  $objdescresumida = $objdesc;

            if( ($boSuperUsuario ||$boPerfilPerm) && ($boSuperUsuario || $estadoPerm) ){
            		$img = '<a href="javascript:addestrategia('.$objetivo[$i]['pseid'].');"><img align="absmiddle" src="/imagens/gif_inclui.gif" title="Adicionar Estratégia"></a> <a href="javascript:excluirobjetivo('.$objetivo[$i]['pseid'].');"><img align="absmiddle" src="/imagens/excluir.gif" title="Excluir Objetivo"></a>'; 
           	 		$desc = '&nbsp;&nbsp;<a href="javascript:edit('.$objetivo[$i]['pseid'].');" title="'.$objdesc.'">'.($i + 1).' - '.$objdescresumida.'</a>';
            }else{ 
            	$img = '<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Estratégia">'; 
           	 	$desc = '&nbsp;&nbsp;'.($i + 1).' - '.$objdescresumida;
           	}
           
            ?>d.add(<?= ($i +1)?>,0,'<?=$img?> <b><?=$desc?></b>','','','','/includes/dtree/img/folder.gif','');<?
        
            $sql = "SELECT * FROM pdeescola.planosuporteestrategico    
                    WHERE pseidpai = '{$objetivo[$i]['pseid']}'
                    ORDER BY pseid
                    ";
                     
             $estrategia = $db->carregar( $sql );

           
         	 if(count($estrategia) > 0){
         	 	if(($boSuperUsuario ||$boPerfilPerm) && ($boSuperUsuario || $estadoPerm) ){?>
	             		d.add(<?= ($i +20)?>,<?=$i + 1?>,'<a href="javascript:addestrategia(<?=$objetivo[$i]['pseid']?>);"><img align="absmiddle" src="/imagens/gif_inclui.gif" title="Adicionar Estratégia"></a> - Estratégias');<?
				}else{?>
	          			d.add(<?= ($i +20)?>,<?=$i + 1?>,'<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Estratégia"> - Estratégias');<? 
	          		}
	          }
			  
             for( $k = 0; $k < count($estrategia); $k++ ){
                 if ( $estrategia[$k]['pseid'] != '' )
                 {
                  $cont++;
        		  ?>
	             // d.add(<?=$estrategia[$k]['pseid'] - 1?>,<?=$i + 1?>,'<a href="javascript:addestrategia();"><img align="absmiddle" src="/imagens/gif_inclui.gif" title="Adicionar Estratégia"></a> - Estratégias');
	        	  <?
                  $num = (($i + 1).'.'.($k + 1));
                  
                  if( ($boSuperUsuario ||$boPerfilPerm) && ($boSuperUsuario || $estadoPerm) ){
                  	$img = '<a href="javascript:addmeta('.$estrategia[$k]['pseid'].');"> <img align="absmiddle" src="/imagens/gif_inclui.gif" title="Adicionar Meta"></a>';
                  }else{
                   	$img = '<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Meta">';
                  }
                  
                    $estdesc =  str_replace(chr(10),"",str_replace(chr(13)," ",addslashes($estrategia[$k]['psedescricao'])));
                    if (strlen($estdesc)>90) $estdescresumida = substr($estdesc,0,90).'...'; else  $estdescresumida = $estdesc;

                 if( ($boSuperUsuario ||$boPerfilPerm) && ($boSuperUsuario || $estadoPerm) ){
                  		$desc = '<title="Alterar/Excluir Estrategia"><a href="javascript:excluirestrategia('.$estrategia[$k]['pseid'].');"><img align="absmiddle" src="/imagens/excluir.gif" title="Excluir Critério"></a>&nbsp;&nbsp;<a href="javascript:edit('.$estrategia[$k]['pseid'].');" title="'.$estdesc.'">'.$num.' - '.$estdescresumida.'</a></a>';
                 }else{
                  		$desc = '<title="Alterar/Excluir Estrategia">&nbsp;&nbsp;'.$num.' - '.$estdescresumida;
                 }
                  	
                  ?>d.add( <?=$estrategia[$k]['pseid']?>, <?=$i + 1?>,'<?=$img?> <b><?=$desc?></b>','','','','/includes/dtree/img/folder.gif','');<?
                 
                  $sql = "SELECT * FROM pdeescola.planosuporteestrategico    
                          WHERE pseidpai = '{$estrategia[$k]['pseid']}'
                          ORDER BY pseid
                    ";
                     
                  $meta = $db->carregar( $sql );
                               
                  if(count($meta[0]) > 0){
                  	if( ($boSuperUsuario ||$boPerfilPerm) && ($boSuperUsuario || $estadoPerm) ){?>
                  			d.add(<?=$i+30?>,<?=$estrategia[$k]['pseid']?>,'<a href="javascript:addmeta(<?=$estrategia[$k]['pseid']?>);"><img align="absmiddle" src="/imagens/gif_inclui.gif" title="Adicionar Metas"></a> - Metas');<?
				  	}else{?>
				  		d.add(<?=$i+30?>,<?=$estrategia[$k]['pseid']?>,'<img align="absmiddle" src="/imagens/gif_inclui.gif" title="Não é permitido adicionar Metas"> - Metas');		
				  <?}
		          }
				  
                  for( $j = 0; $j < count($meta); $j++ ){
                      if ( $meta[$j]['pseid'] != '' ){
                           
	                      $cont_2++;
	                      ?>
			              //d.add(<?=$meta[$j]['pseid']-10?>,<?=$estrategia[$k]['pseid']?>,'<a href="javascript:addmeta();"><img align="absmiddle" src="/imagens/gif_inclui.gif" title="Adicionar Metas"></a> - Metas');
			        	  <?
	                      $num = (($i + 1).'.'.($k + 1).'.'.($j + 1));
	 
	                      $img = ''; 
	                      
	                      $metadesc =  str_replace(chr(10),"",str_replace(chr(13)," ",addslashes($meta[$j]['psedescricao'])));
	                      if (strlen($metadesc)>90) $metadescresumida = substr($metadesc,0,90).'...'; else  $metadescresumida = $metadesc;
	                     
	                     if(($boSuperUsuario ||$boPerfilPerm) && ($boSuperUsuario || $estadoPerm) ){
	                     	$desc = ' <a href="javascript:abrirPlanoDeAcao('.$meta[$j]['pseid'].');"><img align="absmiddle" src="/imagens/gif_inclui.gif" title="Visualizar Plano de Ação"></a>          <title="Alterar/Excluir Meta"><a href="javascript: excluirmeta('.$meta[$j]['pseid'].');"><img align="absmiddle" src="/imagens/excluir.gif" title="Excluir Critério"></a>&nbsp;&nbsp;<a href="javascript:editMeta('.$meta[$j]['pseid'].');" title="'.$metadesc.'">'.$num.' - '.$metadescresumida.'</a></a><a href="javascript:abrirPlanoDeAcaoReport('.$meta[$j]['pseid'].');"><img align="absmiddle" src="/imagens/consultar.gif" title="Consulta Plano de Ação"></a>';
						 }else{
	                      		$desc = ' <img align="absmiddle" src="/imagens/gif_inclui.gif" title="Visualizar Plano de Ação"><title="Alterar/Excluir Meta">&nbsp;&nbsp;'.$num.' - '.$metadescresumida.'<a href="javascript:abrirPlanoDeAcaoReport('.$meta[$j]['pseid'].');"><img align="absmiddle" src="/imagens/consultar.gif" title="Consulta Plano de Ação"></a>';
	                     }?>
	                     
	                     d.add( <?=$meta[$j]['pseid']?>, <?=$estrategia[$k]['pseid']?>,'<?=$img?> <b><?=$desc?></b>','','','','/includes/dtree/img/folder.gif','');<?
	  
                      }
                   } 

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

//pdeescola.php?modulo=principal/sintese_autoavaliacao/objetivos_estrategias&acao=A

function addobjetivo()
{
    return windowOpen('pdeescola.php?modulo=principal/sintese_autoavaliacao/popupObjetivosMetas&acao=I&tipo=objetivo' + '&pdeid=<?=$_SESSION['pdeid'];?>',
                      'popupObjetivosMetas',
                      'height=240,width=750,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}


function addestrategia(idpai )
{
	 return windowOpen('pdeescola.php?modulo=principal/sintese_autoavaliacao/popupObjetivosMetas&acao=I&tipo=estrategia&pseidpai=' + idpai + '&pdeid=<?=$_SESSION['pdeid'];?>',
                      'popupObjetivosMetas',
                      'height=400,width=750,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}


function addmeta(idpai )
{
	 return windowOpen('pdeescola.php?modulo=principal/sintese_autoavaliacao/popupObjetivosMetas&acao=I&tipo=meta&pseidpai='+ idpai + '&pdeid=<?=$_SESSION['pdeid'];?>',
                      'popupObjetivosMetas',
                      'height=400,width=750,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}


function edit(id)
{
	 return windowOpen('pdeescola.php?modulo=principal/sintese_autoavaliacao/popupObjetivosMetas&acao=I&edit='+id + '&pdeid=<?=$_SESSION['pdeid'];?>',
                      'popupObjetivosMetas',
                      'height=400,width=750,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function editMeta(id)
{
	 return windowOpen('pdeescola.php?modulo=principal/sintese_autoavaliacao/popupObjetivosMetas&acao=I&tipo=meta&edit='+id + '&pdeid=<?=$_SESSION['pdeid'];?>',
                      'popupObjetivosMetas',
                      'height=400,width=750,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function abrirPlanoDeAcao(id)
{
  
    return window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/planoDeAcao&acao=A&pseid='+ id + '&pdeid=<?=$_SESSION['pdeid'];?>';
      
}

function abrirPlanoDeAcaoReport(id)
{
  
    return window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/planoDeAcaoReport&acao=A&pseid=' + id + '&pdeid=<?=$_SESSION['pdeid'];?>';
      
}

function verificarGerente(id)
{

}

function excluirestrategia(id )
{
     if (confirm('Deseja excluir o registro?')) 
     {
         return window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A&excluir=' + id;
     }
}

function excluirobjetivo(id)
{
     if (confirm('Deseja excluir o registro?')) 
     {
     	return window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A&excluir=' + id;
     }     
}

function excluirmeta(id)
{ 
     if (confirm('Deseja excluir o registro?')) 
     {
       return  window.location.href = 'pdeescola.php?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A&excluir=' + id;
     
     }
   
    
}

function addatividade(id)
{
	janela('geral/popup_objetivoprog.php?tipo=3&idpai='+id, 500,180);
}

function editobjetivo(id)
{
	janela('geral/popup_objetivoprog.php?tipo=1&id='+id, 500,180);
}

function editestrategia(id)
{
	janela('geral/popup_objetivoprog.php?tipo=2&id='+id, 500,180);
}

function editatividade(id)
{
	janela('geral/popup_objetivoprog.php?tipo=3,&id='+id, 500,180);
}

function cadastraJustificativa()
{
	     return window.open('pdeescola.php?modulo=principal/sintese_autoavaliacao/cadastraJustificativa&acao=A&link=S',
                  'cadastraJustificativa',
                  'height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}
</script>
