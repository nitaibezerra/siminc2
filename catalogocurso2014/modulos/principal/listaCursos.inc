<?php
if($_SESSION['catalogo']){
	unset($_SESSION['catalogo']);
	echo "<script>
			window.location = 'catalogocurso2014.php?modulo=inicio&acao=C';
		  </script>";
}

if($_REQUEST['req']!=''){
	$_REQUEST['req']($_REQUEST);
}

$arrCoords = Array();
if(!$db->testa_superuser()){
	$arrCoords = recuperaCoordenacaoResponssavel();
	$pflcod = pegaPerfil($_SESSION['usucpf']);
}

if(isset($_POST['action']) && !empty($_POST['action'])){
    switch($_POST['action']){
        case 'renderSubarea':
            extract($_POST);
            if(!$_POST['ateid']){
            	$_POST['ateid'] = 0;	
            }
               
            $sql = "SELECT 		suaid AS codigo, 
            					suadesc AS descricao
                    FROM 		catalogocurso2014.subarea
                    WHERE 		ateid = '{$_POST['ateid']}' AND subano = {$_SESSION['exercicio']}
                    ORDER BY 	suadesc;";
            
            $db->monta_combo('suaid', $sql, 'S', 'Selecione...', '', '', 'Subarea', '450', 'N', 'suaid'); ?>
            <script language="javascript">
                $('#suaid').change(function(){
                    var data = {action: 'renderEspecialidade' , suaid: $(this).val()};
                    $.post(window.location.href, data , function(html){
                        $('#container_select_especialidade').hide().html(html).fadeIn();
                    });
                });
            </script>
            <?php
            break;
        case 'renderEspecialidade':
            extract($_POST);

            if(!$_POST['suaid']){
                $_POST['suaid'] = 0;
            }
            
            $sql = "SELECT 		espid AS codigo, 
            					espdesc AS descricao
            		FROM 		catalogocurso2014.especialidade
            		WHERE 		suaid = '{$_POST['suaid']}' AND espano = {$_SESSION['exercicio']}
            		ORDER BY 	espdesc;";
            $db->monta_combo('espid', $sql, 'S', 'Selecione...', '', '', 'Especialidade', '450', 'N', 'espid');
            break;
    }
    exit();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

monta_abas();

extract($_POST); ?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_lista_cursos.js"></script>
<script type="text/javascript" src="geral/funcoes.js"></script>

<form method="post" enctype="multipart/form-data" name="frmCatalogo" id="frmCatalogo">
<?php if($_SESSION['catalogo']['curid']){ ?>
 <div class="row">
	<div class="col-md-12">
    	<ul class="breadcrumb">
        	<li>
            	<strong>Curso: </strong>
                <?php echo $_SESSION['catalogo']['curid'] .' - ' . $_SESSION['catalogo']['curdesc']; ?>
            </li>
        </ul>
    </div>
</div>
<?php } ?>
<table align="center" width="98%" border="0" class="tabela"  cellpadding="5" cellspacing="1">
	<tr>
		<td class="SubTituloDireita">Área</td>
		<td><?php monta_combo_areaTematica_pesquisa( $_POST ); ?></td>
	</tr>
    <tr>
        <td class="SubTituloDireita" width="15%">Subárea</td>
        <td id="container_select_subarea">
            <select class="CampoEstilo" id="suaid" name="suaid" data-placeholder="Selecione" style="width:450px;"><option>Selecione uma Área...</option></select>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="15%">Especialidade</td>
        <td id="container_select_especialidade">
            <select class="CampoEstilo" id="espid" name="espid" data-placeholder="Selecione" style="width:450px;"><option>Selecione uma Subárea...</option></select>
        </td>
    </tr>
	<tr>
		<td class="SubTituloDireita">Código do Curso&nbsp;</td>
		<td>
			<?php $curid = $_POST['curid'];?>
			<?php echo campo_texto('curid', 'N', 'S', 'Código do Curso', '10', '10','',''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome do Curso&nbsp;</td>
		<td>
			<?php $curdesc = $_POST['curdesc']; ?>
			<?php echo campo_texto('curdesc', 'N', 'S', 'Nome do Curso', '68', '255','',''); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nivel do Curso&nbsp;</td>
		<td><?php monta_combo_nivelCurso_pesquisa( $_POST ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Coordenação responsável no<br> MEC ou na CAPES</td>
		<td>
			<?php
			if( !$db->testa_superuser() ){
				$arrCoords = recuperaCoordenacaoResponssavel();
			}
			
			$aryWhere[] = "coordstatus = 'A'";
			$aryWhere[] = "coorano = {$_SESSION['exercicio']}";
			
			if(is_array($arrCoords) && count($arrCoords)>0){
				$coords = implode(",",$arrCoords);
				$aryWhere[] = "coordid IN ({$coords})";
			}
			
			$sql = "SELECT		coordid as codigo,
								coordsigla||' - '||coorddesc as descricao
					FROM		catalogocurso2014.coordenacao
								".(is_array($aryWhere) ? ' WHERE ' . implode(' AND ', $aryWhere) : '')."
					ORDER BY	coordsigla";
			
			$coordid = $_REQUEST['coordid'];
			$db->monta_combo('coordid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Coordenação responsável no MEC', '450', 'N', 'coordid'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita"> </td>
		<td>
			<input type="button" id="pesquisar" value="Pesquisar"/>
		</td>
	</tr>
	<?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){ ?>
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:center;font-weight:bold;font-size:12px;" colspan="2">
			<span style="float:left;font-weight:bold;">
				<img border="0" align="absmiddle" onclick="window.location = 'catalogocurso2014.php?modulo=principal/cadCatalogo&acao=A';" title="Gerar Guia de Tramitações Antigas" style="cursor: pointer" src="../imagens/gif_inclui.gif">
				Adicionar Curso
			</span>
		</td>
	</tr>
	<?php } else { 
	$perfil = pegaPerfilGeral($_SESSION['usucpf']);
	
	if(in_array(PERFIL_SUPERUSUARIO,$perfil) || in_array(PERFIL_ADMINISTRADOR,$perfil)){ ?>
	<tr>
		<td style="background-color:#e9e9e9; color:#404040; width:600px; text-align:center;font-weight:bold;font-size:12px;" colspan="2">
			<span style="float:left;font-weight:bold;">
				<img border="0" align="absmiddle" onclick="window.location = 'catalogocurso2014.php?modulo=principal/cadCatalogo&acao=A';" title="Gerar Guia de Tramitações Antigas" style="cursor: pointer" src="../imagens/gif_inclui.gif">
				Adicionar Curso
			</span>
		</td>
	</tr>	
	<?php } 
	 } ?>
</table>

<input type="hidden" value="" id="req" name="req"/>
</form>

<script language="javascript" type="text/javascript">
    $('#ateid').change(function(){
        var data = {action: 'renderSubarea' , ateid: $(this).val()};
        $.post(window.location.href, data , function(html){
            $('#container_select_subarea').hide().html(html).fadeIn();
        });

        var data = {action: 'renderSubarea' , ateid: 0};
        $.post(window.location.href, data , function(html){
            $('#container_select_especialidade').hide().html(html).fadeIn();
        });
    });

    <?php if($ateid){ if(!$suaid) $suaid = "''" ?>
   		var data = {action: 'renderSubarea' , ateid: <?php echo $ateid; ?> , suaid : <?php echo $suaid; ?> };
    	$.post(window.location.href, data , function(html){
        $('#container_select_subarea').hide().html(html).fadeIn();
    });
    <?php } ?>

    <?php if($ateid && $suaid){  if(!$espid) $espid = "''" ?>
    	var data = {action: 'renderEspecialidade' , suaid: <?php echo $suaid; ?>, espid : <?php echo $espid; ?> };
    	$.post(window.location.href, data , function(html){
        $('#container_select_especialidade').hide().html(html).fadeIn();
    });
    <?php } ?>

	$( document ).ready(function() {
		var exercicio = "<h2 style=\"text-align: center;\">Catálogo Curso "+<?php echo $_SESSION['exercicio']; ?>+"</h2>";
	    jQuery('#top_content_label_sistema').html(exercicio);
	});
</script>

<?php listaCursos($_REQUEST); ?>
