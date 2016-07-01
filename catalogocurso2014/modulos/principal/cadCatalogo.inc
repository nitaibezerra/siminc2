<?php
$_REQUEST['curid'] = $_REQUEST['curid'] ? $_REQUEST['curid'] : $_SESSION['catalogo']['curid'];
$_SESSION['catalogo']['curid'] = $_REQUEST['curid'] ? $_REQUEST['curid'] : $_SESSION['catalogo']['curid'];

$perfil = pegaPerfilGeral($_SESSION['usucpf']);

if(in_array(PERFIL_SUPERUSUARIO,$perfil) || in_array(PERFIL_ADMINISTRADOR,$perfil)){
	$habilitado = "S";
} else {
	$habilitado = "N";
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

            if(!$_POST['suaid'])
                $_POST['suaid'] = 0;

            $sql = "SELECT 		espid AS codigo, 
            					espdesc AS descricao
            		FROM 		catalogocurso2014.especialidade
            		WHERE 		suaid = '{$_POST['suaid']}' AND espano = {$_SESSION['exercicio']}
            		ORDER BY	espdesc;";
            
            $db->monta_combo('espid', $sql, 'S', 'Selecione...', '', '', 'Especialidade', '450', 'N', 'espid');
            break;
    }
    exit();
}

if($_REQUEST['req']!=''){
    $_REQUEST['req']($_REQUEST);
}

if($_SESSION['catalogo']['curid']){
    $sql = "SELECT curdesc FROM catalogocurso2014.curso WHERE curid = {$_SESSION['catalogo']['curid']} AND curano = {$_SESSION['exercicio']}";
    $_SESSION['catalogo']['curdesc'] = $db->pegaUm($sql);
}

$dadosCurso = recuperaDadosCurso($_REQUEST);
$pflcods = pegaPerfis($_SESSION['usucpf']);

$docid = prePegarDocid( $_REQUEST['curid'] );

if($_SESSION['catalogo']['curid']){
    $permissoes = testaPermissao();
    
	$sql = "SELECT 	abcdemanda FROM catalogocurso2014.abrangenciacurso WHERE curid = {$_SESSION['catalogo']['curid']}";
	$abrangencia = $db->pegaLinha($sql);    
} else {
    $permissoes['gravar'] = true;
}

if(is_array($dadosCurso)){
    extract($dadosCurso);
    $curchtot = $dadosCurso['curchpre']+$dadosCurso['curchdist'];
}
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

if($_SESSION['catalogo']['curid']){
    monta_abas();
} else {
    monta_abas(57394);
}

switch ($modid){
    case MODALIDADE_PRESENCIAL:
        $limitHr = 360;
        break;
    case MODALIDADE_SEMIPRESENCIAL:
        $limitHr = 180;
        break;
    case MODALIDADE_DISTANCIA:
        $limitHr = 180;
        break;
    default:
        $limitHr = 0;
        break;
}  ?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_curso.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript">
	$(document).ready(function(){
		var exercicio = "<h2 style=\"text-align: center;\">Catálogo Curso "+<?php echo $_SESSION['exercicio']; ?>+"</h2>";
	    jQuery('#top_content_label_sistema').html(exercicio);
	});
</script>

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

<form method="post" enctype="multipart/form-data" name="frmCatalogo" id="frmCatalogo">
	<input type="hidden" value="" id="req" name="req"/>
	<input type="hidden" value="" id="link" name="link"/>
	<input type="hidden" value="<?php echo $_SESSION['catalogo']['curid']; ?>" id="curid" name="curid"/>
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
	    <tr>
	        <td class="SubTituloDireita" width="15%">Área:</td>
	        <td>
	            <?php
	            $sql = "SELECT	    ateid as codigo,
	                   				ateid||' - '||atedesc as descricao
	               		FROM        catalogocurso2014.areatematica
	               		WHERE       atestatus = 'A' AND ateano = {$_SESSION['exercicio']}
	               		ORDER BY	atedesc";
	            $db->monta_combo('ateid', $sql, ($permissoes['gravar'] ? 'S' : 'N'), 'Selecione...', '', '', 'Área Temática', '450', 'S', 'ateid'); ?>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita" width="15%">Subárea:</td>
	        <td id="container_select_subarea">
	            <select id="suaid" class="CampoEstilo" name="suaid" data-placeholder="Selecione" style="width:450px;"><option>Selecione uma Área...</option></select>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita" width="15%">Especialidade:</td>
	        <td id="container_select_especialidade">
	            <select class="CampoEstilo" id="espid" name="espid" data-placeholder="Selecione" style="width:450px;"><option>Selecione uma Subárea...</option></select>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Código / Nome do Curso:</td>
	        <td>
	            <?php echo campo_texto('curid', 'N', 'N', 'Nome do Curso', '6', '6','',''); ?>&nbsp;
	            <?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){ 
	            	echo campo_texto('curdesc', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Nome do Curso', '70', '250','','','', '', '', 'id="curdesc"');
	            } else {
	            	echo campo_texto('curdesc', 'S', $habilitado, 'Nome do Curso', '70', '250','','','', '', '', 'id="curdesc"');
	            } ?>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Nivel do Curso:</td>
	        <td>
	        	<?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){
	        		monta_combo_nivelCurso($_POST);
	        	} else { 
	        		monta_checkbox_nivelCurso($dadosCurso); 
	        	}?>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita" width="15%">Rede:</td>
	        <td>
	            <?php monta_checkbox_rede($dadosCurso); ?>
	            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
	        </td>
	    </tr>
		<?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2015){ ?>
	    <tr>
	    	<td class="SubTituloDireita">Etapa de ensino a que se<br> destina o curso:</td>
	        <td>
	        	<?php combo_popup_etapaEnsino($_POST); ?>
	        </td>
	    </tr>
        <tr>
            <td class="SubTituloDireita">Localização da Escola:</td>
            <td><?php monta_combo_localizacaoEscola($_POST); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Localização Diferenciada da Escola:</td>
            <td><?php monta_combo_localizacaoDiferenciadaEscola($_POST); ?></td>
        </tr>	    
		<?php } ?>	    
	    <tr>
	        <td class="SubTituloDireita">Objetivo:</td>
	        <td><?php echo campo_textarea('curobjetivo', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Objetivo', '75', '4', '5000'); ?></td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Descrição do Curso:</td>
	        <td><?php echo campo_textarea('curementa', 'S', ($permissoes['gravar'] ? 'S' : 'N'), 'Descrição do Curso', '75', '4', '5000'); ?></td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita" width="15%">Metodologia:</td>
	        <td><?php echo campo_textarea('curmetodologia', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Metodologia', '75', '4', '5000'); ?></td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Modalidade de Ensino:</td>
	        <td>
	            <?php monta_checkboxcheckbox_modalidadeCurso($dadosCurso); ?>
	            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
	        </td>
	    </tr >
	    <tr id="container_carga_horaria_distancia">
	        <td class="SubTituloDireita"><?php echo ($_SESSION['exercicio']==ANO_EXERCICIO_2014) ? 'Carga horária presencial:' : 'Carga horária mínima presencial:'; ?></td>
	        <td>
	            <?php
	            if($_SESSION['catalogo']['curid']){
	                $antcurchmim = $db->pegaUm("SELECT curchmim FROM catalogocurso2014.curso WHERE curid = {$_SESSION['catalogo']['curid']}");
	                $antcurchmax = $db->pegaUm("SELECT curchmax FROM catalogocurso2014.curso WHERE curid = {$_SESSION['catalogo']['curid']}");
	            } ?>
	            <input type="hidden" name="antcurchmim"  id="antcurchmim"  value="<?php echo $antcurchmim; ?>"/>
	            <input type="hidden" name="antcurchmax"  id="antcurchmax"  value="<?php echo $antcurchmax; ?>"/>
	            <?php echo campo_texto('curchmim', 'N', $enableCurMin, '', '6', '15', '[#]', '', '', '', '', 'id="curchmim"','','','this.value=mascaraglobal(\'[#]\',this.value);'); ?>&nbsp;
	            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">Horas.
	        </td>
	    </tr>
	    <tr id="container_carga_horaria_presencial" >
	        <td class="SubTituloDireita"><?php echo ($_SESSION['exercicio']==ANO_EXERCICIO_2014) ? 'Carga horária a distância:' : 'Carga horária mínima a distância'; ?></td>
	        <td>
	            <?php echo campo_texto('curchmax', 'N', $enableCurMax, '', '6', '10', '[#]', '', '', '', '', 'id="curchmax"','','','this.value=mascaraglobal(\'[#]\',this.value);'); ?>&nbsp;
	            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">Horas.
	        </td>
	    </tr>
	    <tr >
	        <td class="SubTituloDireita"><?php echo ($_SESSION['exercicio']==ANO_EXERCICIO_2014) ? 'Carga horária total:' : 'Carga horária mínima total:'; ?></td>
	        <td>
	            <?php
	                if(!$curchmim){
	                    $curchmim = 0;
	                }
	                if(!$curchmim){
	                    $curchmax = 0;
	                }
	
	                $curchtotal = $curchmim + $curchmax; ?>
	            <?php echo campo_texto('curchtotal', 'S', 'N', '', '6', '10', '[#]', '', '', '', '', 'id="curchtotal"','','','this.value=mascaraglobal(\'[#]\',this.value);') ?>&nbsp;Horas.
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Número de alunos por turma:</td>
	        <td><?php echo campo_texto('curnumestudanteidealpre', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '15', '10', '[#]', '', '', '', '', 'id="curnumestudanteidealpre"','','','this.value=mascaraglobal(\'[#]\',this.value);'); ?>&nbsp;</td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Periodicidade de Monitoramento:</td>
	        <td>
	            <?php monta_radio_uniMedMonitora($_POST); ?>
	            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Infraestrutura Recomendada:</td>
	        <td><?php echo campo_textarea('curinfra', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Infraestrutura Recomendada', '75', '4', '5000'); ?></td>
	    </tr>
	    <?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2015){ ?>
        <tr>
            <td class="SubTituloDireita">Sala de Recursos Multifuncionais:</td>
            <td>
                <?php monta_radio_multi($_POST); ?>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>	 
        <?php } ?>   
	    <tr>
	        <td class="SubTituloDireita">Documentos Adicionais:</td>
	        <td td="tdAnexos">
	            <img border="0" align="absmiddle" title="Incluir Arquivo" style="cursor: pointer" src="../imagens/gif_inclui.gif" id="addArquivo" />
	            Incluir Arquivo<br><br>
	            <table class="tabela" style="width: 80%" align="left" border="0" cellpadding="5" cellspacing="1" id="arquivos">
	                <tr>
	                    <td class="SubTituloDireita" width="50%">
	                        <center>Descrição</center>
	                    </td>
	                    <td class="SubTituloDireita">
	                        <center>Arquivo</center>
	                    </td>
	                    <td class="SubTituloDireita" width="5%"><center> - </center></td>
	                </tr>
	                <?php echo recuperaArquivos($_REQUEST); ?>
	                <tr id="bordainferior">
	                    <td></td>
	                    <td></td>
	                </tr>
	            </table>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita" width="15%">Tempo de duração:</td>
	        <td>
	            <?php echo campo_texto('curduracao', 'S', ($permissoes['gravar'] ? 'S' : 'N'), '', '', '2', '###', '', '', '', '', 'id="curduracao"','','',''); ?>&nbsp;
	            Mês(es)
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Responsável pela ultima<br> alteração:</td>
	        <td>
	            <?php echo campo_texto('usunome', 'N', 'N', '', '55', '55', '', ''); ?>&nbsp;
	            <?php echo campo_texto('hicdata', 'N', 'N', 'DD/MM/YYYY', '17', '17', '', ''); ?> &nbsp;
	            <a id="btoHistorico">Historico [+]</a>
	            <div id="historico" style="display:none">
	                <?php
	                if($_REQUEST['curid']){
	                    $sql = "SELECT		usunome, 
											to_char(hicdata,'DD/MM/YYYY - HH24:MI:SS') AS data
								FROM		catalogocurso2014.historicocurso h
								INNER JOIN 	seguranca.usuario u ON u.usucpf = h.usucpf
								WHERE		curid = {$_REQUEST['curid']}
								ORDER BY	data DESC";
	                    $hts = $db->carregar($sql);
	                }
	                
	                $hts = $hts ? $hts : Array();
	                $cabecalho = Array("Usuário","Data da Alteração");
	                $db->monta_lista_array($hts, $cabecalho, 50, 20, '', '100%', '',Array());
	                ?>
	            </div>
	        </td>
	    </tr>
	    <?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2015){ ?>
        <tr>
        	<td class="SubTituloDireita" width="15%">Capturar Demanda via PDDE Interativo?</td>
            <td>
            	<input name="abcdemanda" type="radio" value="true" <?php if(($abrangencia['abcdemanda'] == 't')) echo 'checked="checked"'; ?>/> Sim
                <input name="abcdemanda" type="radio" value="false" <?php if(($abrangencia['abcdemanda'] == 'f')) echo 'checked="checked"'; ?>/> Não
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>	    
		<tr>
			<td class="SubTituloDireita">Curso disponivel para demanda social?:</td>
			<td>
				<input id="curpademsocial" type="radio" name="curpademsocial" value="t" <?php echo ($curpademsocial == 't' ? 'checked="checked"' : ''); ?> <?php echo ($permissoes['gravar'] ? '' : 'disabled'); ?>/>&nbsp; Sim &nbsp;
				<input id="curpademsocial" type="radio" name="curpademsocial" value="f" <?php echo ($curpademsocial == 'f' ? 'checked="checked"' : ''); ?> <?php echo ($permissoes['gravar'] ? '' : 'disabled'); ?>/>&nbsp; Não &nbsp;
				<img border="0" src="../imagens/obrig.gif">
			</td>
		</tr>       
		<tr class="tr_demsoc" style="display:none;">
			<td class="SubTituloDireita">Percentual máximo de participantes na demanda social:</td>
			<td>
				<?php echo campo_texto('curpademsocialpercmax', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Público-alvo da demanda social', '3', '3','###', '', '', '', '', 'id="curpademsocialpercmax"'); ?>
				%
				<img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
			</td>
		</tr>
		<tr class="tr_demsoc" style="display:none;">
			<td class="SubTituloDireita">Público-alvo da demanda social:</td>
			<td>
				<?php combo_popup_publicoAlvoDemandaSocial($_POST); ?>
				<img align="middle" border="0" src="../imagens/obrig.gif" style="margin-bottom:7px">
			</td>
		</tr>		 
        <?php } ?>
	    <tr>
	        <td colspan="2">
	            <center>
	                <input type="button" id="voltar"  value="Voltar"/>
	                <input type="button" id="salvar"  value="Salvar" <?php echo (!$permissoes['gravar']  ? 'disabled' : ''); ?>/>
	                <input type="button" id="salvarC" value="Salvar e Continuar" <?php echo (!$permissoes['gravar']  ? 'disabled' : ''); ?>/>
	                <input type="button" id="proximo" value="Próximo"/>
	            </center>
	        </td>
	    </tr>
	</table>
</form>

<script language="javascript">
    function selectAllOptionsCatalogo( campo_select ){
        if ( !campo_select ){
            return;
        }
        var j = campo_select.options.length;
        for ( var i = 0; i < j; i++ ){
            campo_select.options[i].selected = true;
        }
    }

    $('#curchmim').change(function(){cargaHorariaTotal()});
    $('#curchmax').change(function(){cargaHorariaTotal()});
    cargaHorariaTotal();

	function cargaHorariaTotal(){
        if($('#curchmim').val()){
        	curchmim = parseInt($('#curchmim').val())
        } else {
        	curchmim = 0;
        }

        if($('#curchmax').val()){
            curchmax = parseInt($('#curchmax').val());
        } else {
        	curchmax = 0;
        }
    	$('#curchtotal').val( +  curchmim + curchmax );
	}

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

    <?php if($ateid): if(!$suaid) $suaid = "''" ?>
        var data = {action: 'renderSubarea' , ateid: <?php echo $ateid; ?> , suaid : <?php echo $suaid; ?> };
        $.post(window.location.href, data , function(html){
            $('#container_select_subarea').hide().html(html).fadeIn();
        });
    <?php endif ?>

    <?php if($ateid && $suaid):  if(!$espid) $espid = "''" ?>
        var data = {action: 'renderEspecialidade' , suaid: <?php echo $suaid; ?>, espid : <?php echo $espid; ?> };
        $.post(window.location.href, data , function(html){
            $('#container_select_especialidade').hide().html(html).fadeIn();
        });
    <?php endif ?>

    <?php if(!$permissoes['gravar']){ ?>
    setTimeout(function(){
        $('select , textarea , input').attr('disabled' , 'disabled');
        $('input:button').removeAttr('disabled' , 'disabled');
        $('#salvar , #salvarC , #bt_salvar_perfil').attr('disabled' , 'disabled');

    } , 500);
    <?php } ?>

    jQuery(document).ready(function(){
		<?php if($_SESSION['exercicio']==ANO_EXERCICIO_2015){ ?>
		$('#curpademsocialpercmax').keyup(function(){
			if( parseInt($(this).val()) > 100 ){
				$(this).val('100');
			}
		});
		
		$('[name="curpademsocial"]').click(function(){
			if($(this).val()=="t"){
				$('.tr_demsoc').show();
			} else {
				$('.tr_demsoc').hide();
			}
		});
		
		if($('[name="curpademsocial"]:checked').val()=="t"){
			$('.tr_demsoc').show();
		} else {
			$('.tr_demsoc').hide();
		}
		<?php } ?>    
    
		$('#salvar').click(function(){
			selectAllOptionsCatalogo(document.getElementById('cod_etapa_ensino'));
			if(jQuery('#ateid').val() == ''){
				alert('O campo "Área" é obrigatório!');
				jQuery('#ateid').focus();
				return false;
			}		
			
			if(jQuery('#curdesc').val() == ''){
				alert('O campo "Nome do Curso" é obrigatório!');
				jQuery('#curdesc').focus();
				return false;
			}		
			
			if(jQuery('#ncuid').val() == ''){
				alert('O campo "Nivel do Curso" é obrigatório!');
				jQuery('#ncuid').focus();
				return false;
			}	
			
			if(jQuery('[name=redid[]]:checked').length <= 0){
				alert('O campo "Rede" é obrigatório!');
				jQuery('[name=redid[]]').focus();
				return false;
			}		
			
			if(jQuery('#curobjetivo').val() == ''){
				alert('O campo "Objetivo" é obrigatório!');
				jQuery('#curobjetivo').focus();
				return false;
			}	
			
			if(jQuery('#curementa').val() == ''){
				alert('O campo "Descrição do Curso" é obrigatório!');
				jQuery('#curementa').focus();
				return false;
			}			
			
			if(jQuery('[name=modid[]]:checked').length <= 0){
				alert('O campo "Modalidade de Ensino" é obrigatório!');
				jQuery('[name=modid[]]').focus();
				return false;
			}				
	
			if(jQuery('#curchmim').val() == ''){
				alert('O campo "Carga horária presencial" é obrigatório!');
				jQuery('#curchmim').focus();
				return false;
			}			
	
			if(jQuery('#curchmax').val() == ''){
				alert('O campo "Carga horária distância" é obrigatório!');
				jQuery('#curchmax').focus();
				return false;
			}			
		
			if(jQuery('#curnumestudanteidealpre').val() == ''){
				alert('O campo "Número de alunos por turma" é obrigatório!');
				jQuery('#curnumestudanteidealpre').focus();
				return false;
			}				
			
			if(jQuery('[name=uteid]:checked').length <= 0){
				alert('O campo "Periodicidade de Monitoramento" é obrigatório!');
				jQuery('[name=uteid]').focus();
				return false;
			}					
			
			<?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2015){  ?>
			selectAllOptionsCatalogo(document.getElementById('padid'));
			if(jQuery('[name=cursalamulti]:checked').length <= 0){
				alert('O campo "Sala de Recursos Multifuncionais" é obrigatório!');
				jQuery('[name=cursalamulti]').focus();
				return false;
			}	 				
			<?php } ?>
			
			if(jQuery('#curduracao').val() == ''){
				alert('O campo "Tempo de duração" é obrigatório!');
				jQuery('#curduracao').focus();
				return false;
			}	
			
			<?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2015){ ?>
	    	var p = document.getElementsByName('padid[]')[0];
	    	var publico = '';		
	    	
	    	if (p.options.length > 0){
	    		for (var i=0; i<p.options.length; i++){
		    		if (p.options[i].value != ''){
		    			publico += "'" + p.options[i].value + "',";
		    		}
	    		}      
	    	}   			
			
			if(jQuery('[name=abcdemanda]:checked').length <= 0){
				alert('O campo "Capturar Demanda via PDDE Interativo?" é obrigatório!');
				jQuery('[name=abcdemanda]').focus();
				return false;
			}			

			if($('[name="curpademsocial"]:checked').val()=="t"){
				if(jQuery('#curpademsocialpercmax').val() == ''){
					alert('O campo "Percentual máximo de participantes na demanda social" é obrigatório!');
					jQuery('#curpademsocialpercmax').focus();
					return false;
				}
				
				if(publico == ''){
					alert('O campo "Público-alvo da demanda social" é obrigatório!');
					jQuery('#padid').focus();
					return false;
				}  		
			}	
			<?php } ?>

			//Valida Numero de estudantes por turma
			var mim = parseInt($('#curnumestudanteminpre').val());
			var ideal = parseInt($('#curnumestudanteidealpre').val());
			var max = parseInt($('#curnumestudantemaxpre').val());
	
			if(((mim>ideal)||(mim>max) || (ideal>max))&&modid!='3'){
				alert('Numero de estudantes por turma inválido. (Dica: Mínimo < Ideal < Máximo)');
				$(this).removeAttr('disabled');
				return false;
			}
	
			mim = parseInt($('#curnumestudantemindist').val());
			ideal = parseInt($('#curnumestudanteidealdist').val());
			max = parseInt($('#curnumestudantemaxdist').val());
	
			if((mim>ideal)||(mim>max) || (ideal>max)&&modid!='1'){
				alert('Numero de estudantes por turma inválido. (Dica: Mínimo < Ideal < Máximo)');
				$(this).removeAttr('disabled');
				return false;
			}
	
			$('#req').val('salvarCatalogo');
			$('#frmCatalogo').submit();
		});
    });
</script>