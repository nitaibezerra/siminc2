<?php
$curid = $_SESSION['catalogo']['curid'];
// Publico alvo

if(!$_SESSION['catalogo']['curid']){
	$al = array('alert' => 'Verifique sua Sessão!', 'location' => 'catalogocurso2014.php?modulo=principal/listaCursos&acao=A');
	alertlocation($al);
}

if(isset($_POST['action']) && !empty($_POST['action'])){
    $action = $_POST['action'];
    if($action == 'render_lista_tipo'){

        if($_POST['perfil']){
            $idPerfil = $_POST['perfil'];
        } else {
            $idPerfil = '';
        }

        if($_POST['tipo_lista']){
            $tipoLista = $_POST['tipo_lista'];
        } else {
            $tipoLista = '';
        }

        if($tipoLista && $idPerfil){

            switch($tipoLista){
                case 1:
                    $sql = "SELECT  	pk_cod_area_ocde AS codigo,
                        				no_nome_area_ocde AS descricao
                    		FROM    	educacenso_".(ANO_CENSO).".tab_area_ocde";
                    break;
                case 2:
                    $sql = "SELECT 		pk_cod_disciplina AS codigo,
                                		no_disciplina AS descricao
                            FROM        educacenso_".(ANO_CENSO).".tab_disciplina";
                    break;
                case 3:
                    $sql = "SELECT      pk_cod_etapa_ensino AS codigo,
                                		no_etapa_ensino AS descricao
                            FROM	    educacenso_".(ANO_CENSO).".tab_etapa_ensino
                            ORDER BY    cod_etapa_ordem";
                    break;
            }
            if($sql){
                $listaTipo = $db->carregar($sql);
            }
        } ?>
		<select id="lista_tipo" name="lista_tipo" class="form-control chosen-select" required="required" data-placeholder="Selecione...">
        	<option value=""></option>
            <?php if($listaTipo): foreach($listaTipo as $tipo): ?>
            	<option value="<?php echo $tipo['codigo']; ?>" <?php if($_POST['id'] == $tipo['codigo']) echo 'selected="selected"' ?>><?php echo $tipo['descricao']; ?></option>
            <?php endforeach; endif; ?>
        </select>
        <script language="javascript">
            setTimeout(function(){
                $('.chosen-select').chosen({allow_single_deselect:true});
            }, 100);
        </script>
		<?php
        exit();
    } 
}

if($_REQUEST['req']!=''){
	$_REQUEST['req']($_REQUEST);
}

$_REQUEST['curid'] = $_REQUEST['curid'] ? $_REQUEST['curid'] : $_SESSION['catalogo']['curid'];

if($_SESSION['catalogo']['curid']){
	$permissoes = testaPermissao();
} else {
	$permissoes['gravar'] = true;
}

if($_SESSION['exercico']==ANO_EXERCICIO_2014){
	$dadosPublico = recuperaDadosPublicoAlvo($_REQUEST);
	if(is_array($dadosPublico)){
		foreach($dadosPublico as $k => $dado){
			$$k = $dado;
		}
	}
}

$_POST['cod_mod_ensino'] = recuperaRedesCurso($_REQUEST);
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

monta_abas();

$dataForm = array('pafid' => '' , 'perfil' => '', 'tipo_lista' => '' , 'lista_tipo' => '');
if(isset($_REQUEST['pafid']) && !empty($_REQUEST['pafid'])){
    $pafid = $_REQUEST['pafid'];

    $sql = "SELECT 		pafid, fexid AS perfil, pafoutrasexigencias
            FROM 		catalogocurso2014.publicoalvo_assocfuncaoexercida WHERE pafid = {$pafid}";
    $perfil = $db->pegaLinha($sql);
    
    $sqlAreaFormacao = "SELECT 		a.pk_cod_area_ocde AS codigo, 
    								af.no_nome_area_ocde AS descricao 
    					FROM 		catalogocurso2014.publicoalvo_assocareaformacao a
			            INNER JOIN 	educacenso_".(ANO_CENSO).".tab_area_ocde af ON af.pk_cod_area_ocde = a.pk_cod_area_ocde
                        WHERE 		pafid = {$pafid}";
    $pk_cod_area_ocde = $db->carregar($sqlAreaFormacao);

    $sqlDisciplina = "SELECT 		d.pk_cod_disciplina AS codigo, 
    								d.no_disciplina AS descricao
    				  FROM 			catalogocurso2014.publicoalvo_assocdisciplina pad
                      INNER JOIN 	educacenso_".(ANO_CENSO).".tab_disciplina d ON d.pk_cod_disciplina = pad.pk_cod_disciplina
                      WHERE 		pafid = {$pafid}";
    $pk_cod_disciplina = $db->carregar($sqlDisciplina);

    $sqlEtapaEnsino = "SELECT 		ee.pk_cod_etapa_ensino AS codigo, 
    								no_etapa_ensino AS descricao 
    				   FROM 		catalogocurso2014.publicoalvo_assocetapaensino e
                       INNER JOIN 	educacenso_".(ANO_CENSO).".tab_etapa_ensino ee on ee.pk_cod_etapa_ensino = e.pk_cod_etapa_ensino
                       WHERE 		pafid = {$pafid}";
    $pk_cod_etapa_ensino = $db->carregar($sqlEtapaEnsino);

    $sqlNivel = "SELECT nivelescolaridadeid FROM catalogocurso2014.publicoalvo_assocnivelescolaridade WHERE pafid = {$pafid}";
    $nivelEscolaridadeAssoc = $db->carregarColuna($sqlNivel);

    $sqlModeEnsino = "SELECT modensinoid FROM catalogocurso2014.publicoalvo_assocmodensino WHERE pafid = {$pafid}";
    $modEnsino = $db->carregarColuna($sqlModeEnsino);

    if(!$areaFormacaoAssoc){
        $areaFormacaoAssoc = array();
    }

    if(!$disciplinaAssoc){
        $disciplinaAssoc = array();
    }

    if(!$etapaEnsinoAssoc){
        $etapaEnsinoAssoc = array();
    }

    if(!$modEnsino){
        $modEnsino = array();
    }

    if(!$nivelEscolaridadeAssoc){
        $nivelEscolaridadeAssoc = array();
    }

    if($perfil){
        $dataForm = $perfil;
    }

} else {
    $pafid = '';
    $nivelEscolaridadeAssoc = array();
    $perfil= array();
} ?>
<div class="row">
    <div class="col-md-12">
        <ul class="breadcrumb">
            <li>
                <strong>Curso: </strong>
                <?php echo $_SESSION['catalogo']['curid'] .' - ' . $_SESSION['catalogo']['curdesc'] ?>
            </li>
        </ul>
    </div>
</div>
<?php
if($perfil){
	$andFexid = "OR fexid = {$perfil['perfil']}";
} else {
    $andFexid = '';
}

$sqlPerfil = "SELECT	fexid AS codigo,
                        fexdesc AS descricao
              FROM      catalogocurso2014.funcaoexercida
              WHERE     fexano = {$_SESSION['exercicio']} AND fexstatus = 'A' 
              			AND fexid NOT IN (SELECT fexid FROM catalogocurso2014.publicoalvo_assocfuncaoexercida WHERE curid = {$curid})
                    	$andFexid
              ORDER BY 	fexdesc";

$perfis = $db->carregar($sqlPerfil); ?>

<link rel='stylesheet' type='text/css' href='/library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>
<script src="/library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="/library/jquery/jquery.form.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="/library/jquery/jquery-ui-1.10.3/jquery-ui.min.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="/library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
<script language="JavaScript" src="/estrutura/js/funcoes.js"></script>
<script src="/includes/JQuery/jquery-1.9.1/funcoes.js" type="text/javascript"></script>
<script type="text/javascript" src="geral/funcoes_publico.js"></script>
<script type="text/javascript" language="javascript">
    <?php if($pafid) echo "carregarListaTipo('{$dataForm['lista_tipo']}');" ?>

    function carregarListaTipo(id){
        perfil = $('#perfil').val();
        tipo_lista = $('#tipo_lista').val();
        data = { action: 'render_lista_tipo' ,  perfil : perfil , tipo_lista: tipo_lista , id:id}
        $.post(window.location.href, data, function(html){
	        $('#container_lista_tipo').hide().html(html).fadeIn();
        });
        $('#container_lista_tipo').fadeIn();
    }

    <?php if(!$permissoes['gravar']){ ?>
    setTimeout(function(){
        $('select , textarea , input').attr('disabled' , 'disabled');
        $('input:button').removeAttr('disabled' , 'disabled');
        $('#salvar , #salvarC , #bt_salvar_perfil, #bt_salvar_perfilcontinuar').attr('disabled' , 'disabled');
    } , 500);
    <?php } ?>

	$(document).ready(function() {
		var exercicio = "<h2 style=\"text-align: center;\">Catálogo Curso "+<?php echo $_SESSION['exercicio']; ?>+"</h2>";
	    jQuery('#top_content_label_sistema').html(exercicio);
	});    
</script>

<table align="center" width="98%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
    <tr class="FundoTitulo">
        <td>
            <b>Associar Perfis</b>
        </td>
    </tr>
</table>

<form method="post" name="frmPerfil" id="frmPerfil">
<input type="hidden" value="salvarPerfil" id="req" name="req"/>
<input type="hidden" value="" id="linkp" name="linkp"/>
<input name="pafid" id="pafid" type="hidden" value="<?php echo $dataForm['pafid']; ?>"/>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td class="SubTituloDireita" width="15%">Perfil</td>
        <td>
            <select name="perfil" id="perfil" class="form-control chosen-select" required="required" data-placeholder="Selecione..." style="width:400px;">
                <option value=""></option>
                <?php foreach ($perfis as $perfil){ ?>
                    <option <?php if( $perfil['codigo'] == $dataForm['perfil']) echo 'selected="selected"'; ?> value="<?php echo $perfil['codigo']; ?>"><?php echo $perfil['descricao']; ?></option>
                <?php } ?>
            </select>
        </td>
    </tr>
    <?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){ ?>
    <tr id="tr_formacao" style="display<?php echo in_array($dadosPublico['pacod_escolaridade'],Array(6,7))?"table-row":"none"; ?>">
        <td class="SubTituloDireita">Área de Formação</td>
        <td>
            <?php
            $sql = "SELECT		pk_cod_area_ocde AS codigo,
                    			no_nome_area_ocde AS descricao
                	FROM        educacenso_".(ANO_CENSO).".tab_area_ocde
                    ORDER BY 	no_nome_area_ocde DESC";

                combo_popup('pk_cod_area_ocde', $sql, 'Todos', '400x400', '',
                    $codigos_fixos = array(), $mensagem_fixo = '', ($permissoes['gravar'] ? 'S' : 'N'), $campo_busca_codigo = false,
                    $campo_flag_contem = false, $size = 4, $width = 400 , $onpop = null, $onpush = null,
                    $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false,
                    $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Disciplina(s) que leciona:</td>
        <td>
            <?php
            $sql = "SELECT		pk_cod_disciplina AS codigo,
								no_disciplina AS descricao
					FROM		educacenso_".(ANO_CENSO).".tab_disciplina
					ORDER BY 	no_disciplina";

            combo_popup('pk_cod_disciplina', $sql, 'Todos', $tamanho_janela = '400x400', $maximo_itens = 0,
                $codigos_fixos = array(), $mensagem_fixo = '', ($permissoes['gravar'] ? 'S' : 'N'), $campo_busca_codigo = false,
                $campo_flag_contem = false, $size = 4, $width = 400 , $onpop = null, $onpush = null,
                $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false,
                $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null); ?>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Etapa de Ensino em que Leciona:</td>
        <td>
            <?php
            $sql = "SELECT		pk_cod_etapa_ensino AS codigo,
								no_etapa_ensino AS descricao
					FROM		educacenso_".(ANO_CENSO).".tab_etapa_ensino
					ORDER BY	no_etapa_ensino";

            combo_popup('pk_cod_etapa_ensino', $sql, 'Todos', '400x400', '',
                '', '', ($permissoes['gravar'] ? 'S' : 'N'), '', '', 4, 400 , $onpop = null, $onpush = null,
                $param_conexao = false, $where=null, $value = null, $mostraPesquisa = true, $campo_busca_descricao = false,
                $funcaoJS=null, $intervalo=false, $arrVisivel = null , $arrOrdem = null); ?>
        </td>
    </tr>
    <?php } ?>
    <tr>
        <td class="SubTituloDireita">Nivel de Escolaridade mínimo Exigido:</td>
        <td>
        	<?php
            $sql = "(SELECT			to_char(pk_cod_escolaridade,'9') AS codigo,
									pk_cod_escolaridade||' - '||no_escolaridade AS descricao,
									h.nivel
				  	 FROM			educacenso_".(ANO_CENSO).".tab_escolaridade e
				  	 INNER JOIN 	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid = e.pk_cod_escolaridade
				  	 WHERE			h.hneano = {$_SESSION['exercicio']})
				  	 UNION ALL
				    (SELECT			to_char(pk_pos_graduacao,'9')||'0' AS codigo,
									pk_pos_graduacao||'0 - '||no_pos_graduacao AS descricao,
									h.nivel
					 FROM			educacenso_".(ANO_CENSO).".tab_pos_graduacao e
				  	 INNER JOIN 	catalogocurso2014.hierarquianivelescolaridade h ON h.nivid::integer = (e.pk_pos_graduacao||'0')::integer
				  	 WHERE			h.hneano = {$_SESSION['exercicio']})";
            
            $niveis = $db->carregar($sql);

            $nv = Array();
            if(is_array($niveis)){
                foreach($niveis as $nivel){
                    if(!in_array($nivel['nivel'],$nv)){
                        array_push($nv, $nivel['nivel']);
                    }
                    $checked = '';
                    if( is_array($nivelEscolaridadeAssoc) ){
                        foreach($nivelEscolaridadeAssoc as $resp){
                            if( $resp == $nivel['codigo'] ){
                                $checked = 'checked="checked"';
                            }
                        }
                    }
                    echo "<br>
						  <input type=\"checkbox\" id=\"".$nivel['nivel']."\" class=\"".implode(" ",$nv)."\" value=\"".$nivel['codigo']."\" $checked ".($permissoes['gravar'] ? '' : 'disabled="disabled"')." name=\"cod_escolaridade[]\"> ".$nivel['descricao'];
                }
            } ?>
        </td>
    </tr>
    <?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){ ?>
    <tr>
        <td class="SubTituloDireita">Modalidade em que leciona:</td>
        <td>
            <?php
            $sql = "SELECT	pk_cod_mod_ensino AS codigo,
                    		no_mod_ensino AS descricao
                	FROM    educacenso_".(ANO_CENSO).".tab_mod_ensino";

                $marcados = $modEnsino;
                $db->monta_checkbox('cod_mod_ensino[]', $sql, $marcados, $separador='  ', Array("disabled" => !$permissoes['gravar'])); ?>
        </td>
    </tr>
    <?php } ?>
    <?php if($_SESSION['exercicio']==ANO_EXERCICIO_2015){ ?>
	<tr>
		<td class="SubTituloDireita">Outras Exigências:</td>
		<td><?php echo campo_textarea('pafoutrasexigencias', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Outras Exigências', '75', '4', '5000','','','','','',$dataForm['pafoutrasexigencias']); ?></td>
	</tr>    
	<?php } ?>
    <tr>
        <td colspan="2" class="FundoTitulo" style="text-align: center;">
            <?php if($dataForm['pafid']){ ?>
            <input  type="button" value="Cancelar" onclick="javascript:window.location.href = '/catalogocurso2014/catalogocurso2014.php?modulo=principal/cadPublicoAlvo&acao=A'" />
            <?php } ?>
			<input type="button" id="bt_salvar_perfil" value="Salvar" <?php echo (!$permissoes['gravar']  ? 'disabled' : ''); ?>/>
			<input type="button" id="bt_salvar_perfilcontinuar" value="Salvar e Continuar" <?php echo (!$permissoes['gravar']  ? 'disabled' : ''); ?>/>
			<input type="button" id="proximo" value="Próximo"/>           
        </td>
    </tr>
</table>
</form>

<?php listarPerfil($curid,'editar'); ?>
    
<?php if($_SESSION['exercicio'] == ANO_EXERCICIO_2014){ ?>

<table align="center" width="98%" border="0" cellpadding="5" cellspacing="1" class="listagem2">
    <tr class="FundoTitulo">
        <td>
            <b>Outros Dados do Público Alvo</b>
        </td>
    </tr>
</table>

<form method="post" name="frmPublicoAlvo" id="frmPublicoAlvo">
	<input type="hidden" value="salvarPublicoAlvo" id="req" name="req"/>
	<input type="hidden" value="" id="link" name="link"/>
	<input type="hidden" value="<?php echo $_REQUEST['curid']; ?>" id="curid" name="curid"/>
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
        <tr>
            <td class="SubTituloDireita">Etapa de ensino a que se destina:</td>
            <td>
                <?php combo_popup_etapaEnsino($_POST); ?>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Sala de Recursos Multifuncionais:</td>
            <td>
                <?php monta_radio_multi($_POST); ?>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
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
		<tr>
			<td class="SubTituloDireita">Outras Exigências:</td>
			<td><?php echo campo_textarea('curpaoutrasexig', 'N', ($permissoes['gravar'] ? 'S' : 'N'), 'Outras Exigências', '75', '4', '5000'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Curso disponivel para demanda social?:</td>
			<td>
				<input type="radio" name="curpademsocial" value="S" <?php echo ($curpademsocial == 't' ? 'checked="checked"' : ''); ?> <?php echo ($permissoes['gravar'] ? '' : 'disabled'); ?>/>&nbsp; Sim &nbsp;
				<input type="radio" name="curpademsocial" value="N" <?php echo ($curpademsocial == 'f' ? 'checked="checked"' : ''); ?> <?php echo ($permissoes['gravar'] ? '' : 'disabled'); ?>/>&nbsp; Não &nbsp;
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
		<tr class="FundoTitulo">
			<td colspan="2">
				<center>
					<input type="button" id="voltar"  value="Voltar"/>
					<input type="button" id="salvar" value="Salvar" <?php echo (!$permissoes['gravar']  ? 'disabled' : ''); ?>/>
					<input type="button" id="salvarC" value="Salvar e Continuar" <?php echo (!$permissoes['gravar']  ? 'disabled' : ''); ?>/>
					<input type="button" id="proximo" value="Próximo"/>
				</center>
			</td>
		</tr>
	</table>
</form>

<?php } ?>
