<?PHP

    if ($_REQUEST['requisicao'] == 'carregar_municipio') {
        carregarMunicipiosPorUF($_POST);
        exit();
    }
    
    if($_REQUEST['requisicao'] == 'excluir'){
        header('Content-Type: text/html; charset=iso-8859-1');
        excluirProcesso($_REQUEST['prcid']);
        listarProcesso();
        exit();
    }
    
    if($_REQUEST['requisicao'] == 'pesquisar'){
        header('Content-Type: text/html; charset=iso-8859-1');
        listarProcesso( $_POST );
        exit();
    }

    if($_POST['tipo'] == 'xls'){
        header('Content-Type: text/html; charset=iso-8859-1');
        listarProcesso( $_POST, $_POST['tipo'] );
        exit();
    }

    include  APPRAIZ."includes/cabecalho.inc";

    $titulo_modulo = "Lista de Processos";
    $modulo = "CPROC";
    monta_titulo( $titulo_modulo, $modulo );

    unset($_SESSION['cproc']['prcid']);
?>

<br>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script language="javascript" type="text/javascript">

    $('.adicionar').live('click',function(){
        window.location.href = "cproc.php?modulo=principal/cad_dados_processo&acao=A";
    });

    function carregarMunicipiosPorUF(estuf) {
        divCarregando();
        jQuery.ajax({
            type: 'POST',
            url: 'cproc.php?modulo=principal/lista_grid_processo&acao=A',
            data: {
                requisicao: 'carregar_municipio', 
                estuf: estuf
            },
            async: false,
            success: function(data){
                jQuery('#td_municipio').html(data);
                divCarregado();
            }
        });
    }
    
    function excluirProcesso(prcid){
        if(confirm("Deseja realmente excluir o processo?")){
            divCarregando();
            $.ajax({
                url: 'cproc.php?modulo=principal/lista_grid_processo&acao=A',
                data:{
                    requisicao: 'excluir',
                    prcid: prcid
                },
                async: false,
                success: function(data) {
                    alert('Processo excluído com sucesso!');
                    $("#div_lista_processo").html(data);
                    divCarregado();
                }
            });
        }
    }

    function pesquisarProcesso(){
        divCarregando();
        $.ajax({
            type: 'POST',            
            url: 'cproc.php?modulo=principal/lista_grid_processo&acao=A&requisicao=pesquisar',
            data: $('#formulario').serialize(),
            async: false,
            success: function(data) {
                $("#div_lista_processo").html(data);
                divCarregado();
            }
        });
    }

    function exportarXls() {
        $('#tipo').val('xls');
        $('#formulario').submit();
    }


    function alterarProcesso(prcid){
        window.location.href = 'cproc.php?modulo=principal/cad_dados_processo&acao=A&prcid='+prcid;
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="tipo" name="tipo" value=""/>
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
        <tr>
            <td class="subtituloDireita" width="20%" colspan="2">Nº Controle SIMEC:</td>
            <td>
                <?PHP
                    echo campo_texto('prcid', 'N', 'S', 'Nº Controle SIMEC', '46', '25', '', '', '', '', '', 'id="prcid"', '', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Tipo de Processo:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  tprid AS codigo,
                                tprdsc AS descricao
                        FROM cproc.tipoprocesso
                        ORDER BY tprdsc
                    ";
                    $db->monta_combo('tprid', $sql, 'S', 'Selecione...', '', '', '', '455', 'N', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Nº do Processo SIDOC:</td>
            <td>
                <?PHP
                    echo campo_texto('prcnumsidoc', 'N', 'S', 'Nº do Processo SIDOC', '46', '17', '#####.######/####-##', '', '', '', '', 'id="prcnumsicoc"', '', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" rowspan="4" width="20%">Interessado:</td>
        </tr>
        <tr>
            <td class="subtituloDireita" width="12%">Mantenedora:</td>
            <td>
                <?PHP
                    if ($mntid) {
                        $sql = "SELECT mntid AS codigo, mntid ||' - '|| TRIM(mntdsc) AS descricao FROM gestaodocumentos.mantenedoras WHERE mntid = {$mntid}";
                        $rmantenedora = $db->carregar($sql);
                    }

                    $sql = "SELECT mntid AS codigo, mntid ||' - '|| TRIM(mntdsc) AS descricao FROM gestaodocumentos.mantenedoras WHERE 1=1 ";

                    $arCampos = Array(Array('codigo' => 'mntid', 'descricao' => 'Código', 'numeric' => 1), Array('codigo' => 'mntdsc', 'descricao' => 'Mantenedora'));
                    combo_popup('mntid', $sql, 'Mantenedora', '400x400', '1', '', '', 'S', '', '', '2', '450', null, null, false, $arCampos, $rmantenedora, true, false, '', false, null, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Mantida:</td>
            <td>
                <?PHP
                if ($iesid) {
                    $sql = "SELECT iesid AS codigo, iesid ||' - '|| TRIM(iesdsc) AS descricao FROM gestaodocumentos.instituicaoensino WHERE iesid = {$iesid} ORDER BY iesdsc";
                    $rmantida = $db->carregar($sql);
                }

                $sql = "SELECT iesid AS codigo, iesid ||' - '|| TRIM(iesdsc) AS descricao FROM gestaodocumentos.instituicaoensino ORDER BY iesdsc";
                $arCampos = Array(Array('codigo' => 'iesdsc', 'descricao' => 'IES'));
                combo_popup('iesid', $sql, 'IES', '400x400', '1', '', '', 'S', '', '', '2', '450', null, null, false, $arCampos, $rmantida, true, false, '', false, null, null);
                ?>
            </td>
        </tr>
        <tr>
        	<td class="subtituloDireita">Entidade:</td>
            <td>
                <?PHP
	                if ($entid) {
	                	$sql = "SELECT entid AS codigo, TRIM(entdsc) as descricao FROM cproc.entidade WHERE entid = {$entid}";
	                	$rentidade = $db->carregar($sql);
	                }
                
                	$sql = "SELECT entid AS codigo, TRIM(entdsc) as descricao FROM cproc.entidade";	
                	$arCampos = Array(Array('codigo' => 'entid', 'descricao' => 'Código', 'numeric' => 1), Array('codigo' => 'entdsc', 'descricao' => 'Entidade'));
                	combo_popup('entid', $sql, 'Entidade', '500x400', '1', '', '', 'S', '', '', '2', '450', null, null, false, $arCampos, $rentidade, true, false, '', false, null, null);
                    //echo campo_texto('entid', 'N', $habil, 'Entidade', '51', '50', '', '', '', '', '', 'id="entid"', '', '', '');
                ?>
            </td>
        </tr>
        <tr>
        	<td class="subtituloDireita" colspan="2">Coordenador:</td>
        	<td>
        		<?php 
        			$sql = 'SELECT DISTINCT uc.usucpf AS codigo, uc.usunome AS descricao FROM seguranca.usuario uc INNER JOIN cproc.coordenadoresponsavel cr ON uc.usucpf = cr.usucpf';
        			
        			$db->monta_combo('coocpf', $sql, 'S', 'Selecione...', '', '', '', '455', 'N', 'coocpf', '');
        		?>
        	</td>
        </tr>
        <tr>
        	<td class="subtituloDireita" colspan="2">Técnico:</td>
        	<td>
        		<?php 
        			$sql = 'SELECT DISTINCT uc.usucpf AS codigo, uc.usunome AS descricao FROM seguranca.usuario uc INNER JOIN cproc.tecnicoresponsavel cr ON uc.usucpf = cr.usucpf';
        		
        			$db->monta_combo('teccpf', $sql, 'S', 'Selecione...', '', '', '', '455', 'N', 'teccpf', '');
        		?>
        	</td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Curso:</td>
            <td>
                <?PHP
                    echo campo_texto('prccurso', 'N', 'S', 'Curso', '46', '100', '', '', '', '', '', 'id="prccurso"', '', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Local de Oferta:</td>
            <td>
                <?PHP 
                    echo campo_texto('prclocaloferta', 'N', 'S', 'Local de Oferta', '46', '100', '', '', '', '', '', 'id="prclocaloferta"', '', '', ''); 
                ?>
            </td>
        </tr>
        <!-- tr>
            <td class="subtituloDireita" colspan="2">Polo:</td>
            <td>
                <?PHP 
                    echo campo_texto('prcpoloferta', 'N', 'S', 'Polo', '46', '100', '', '', '', '', '', 'id="prcpoloferta"', '', '', ''); 
                ?>
            </td>
        </tr-->
        <tr>
            <td class="subtituloDireita" colspan="2">UF:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  estuf AS codigo, 
                                estuf AS descricao 
                        FROM territorios.estado 
                        ORDER BY estuf
                    ";
                    $db->monta_combo('estuf', $sql, 'S', 'Selecione...', 'carregarMunicipiosPorUF', '', '', '140', 'N', 'estuf', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Município:</td>
            <td id="td_municipio">
                <?PHP 
                    $sql = "
                        SELECT  muncod AS codigo, 
                                mundescricao AS descricao 
                        FROM territorios.municipio 

                        WHERE estuf = '{$estuf}' 
                        ORDER BY mundescricao
                    "; 
                    $db->monta_combo('muncod', $sql, 'S', 'Selecione...', '', '', '', '455', 'N', '', ''); 
                ?>
            </td>
        </tr>
        
        <tr>
            <td class="subtituloDireita" colspan="2">Processo administrativo:</td>
            <td id="td_municipio">
                &nbsp;&nbsp;<input type="radio" value="t" name="processo_administrativo">Sim&nbsp;&nbsp;<input type="radio" value="f"  name="processo_administrativo">Não
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Medida Cautelar:</td>
            <td id="td_municipio">
                &nbsp;&nbsp;<input type="radio" value="t" name="medida_cautelar">Sim&nbsp;&nbsp;<input type="radio" value="f"  name="medida_cautelar">Não
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Medida Saneadora:</td>
            <td id="td_municipio">
                &nbsp;&nbsp;<input type="radio" value="t" name="medida_saneadora">Sim&nbsp;&nbsp;<input type="radio" value="f"  name="medida_saneadora">Não
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Penalidade Aplicada:</td>
            <td id="td_municipio">
                &nbsp;&nbsp;<input type="radio" value="t" name="penalidade_aplicada">Sim&nbsp;&nbsp;<input type="radio" value="f"  name="penalidade_aplicada">Não
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Ministério Público:</td>
            <td id="td_municipio">
                &nbsp;&nbsp;<input type="radio" value="t" name="ministerio_publico">Sim&nbsp;&nbsp;<input type="radio" value="f"  name="ministerio_publico">Não
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Verificação In Loco:</td>
            <td id="td_municipio">
                &nbsp;&nbsp;<input type="radio" value="t" name="in_loco">Sim&nbsp;&nbsp;<input type="radio" value="f"  name="in_loco">Não
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Vistas disponibilizadas:</td>
            <td id="td_municipio">
                &nbsp;&nbsp;<input type="radio" value="t" name="vistas_disponibilizadas">Sim&nbsp;&nbsp;<input type="radio" value="f"  name="vistas_disponibilizadas">Não
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Modalidade EAD:</td>
            <td id="td_municipio">
                &nbsp;&nbsp;<input type="radio" value="t" name="prcstatusead">Sim&nbsp;&nbsp;<input type="radio" value="f"  name="prcstatusead">Não
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Supervisão Especial:</td>
            <td id="td_municipio">
                &nbsp;&nbsp;<input type="radio" value="t" name="prcstatussupesp">Sim&nbsp;&nbsp;<input type="radio" value="f"  name="prcstatussupesp">Não
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Fase:</td>
            <td>
                <?PHP
                $sql = "
                        SELECT  fasid AS codigo,
                                fasdsc AS descricao
                        FROM cproc.fase
                        WHERE fasstatus = 'A'
                        ORDER BY fasdsc
                    ";
                $db->monta_combo('fasid', $sql, 'S', 'Selecione...', '', '', '', '455', 'N', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Status:</td>
            <td>
                <?PHP
                $sql = "
                        SELECT  stsid AS codigo,
                                stsdsc AS descricao
                        FROM cproc.status
                        WHERE stsstatus = 'A'
                        ORDER BY stsdsc
                    ";
                $db->monta_combo('stsid', $sql, 'S', 'Selecione...', '', '', '', '455', 'N', '', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita" colspan="2">Prazo Análise:</td>
            <td>
                <?php
                $pmes = $_REQUEST['pmes'] ? $_REQUEST['pmes'] : $pmes;
                $pano = $_REQUEST['pano'] ? $_REQUEST['pano'] : $pano;
                $sql = "select '1' as codigo, 'Janeiro' as descricao
	  	   						union all
	  	   						select '2' as codigo, 'Fevereiro' as descricao
	  	   						union all
	  	   						select '3' as codigo, 'Março' as descricao
	  	   						union all
	  	   						select '4' as codigo, 'Abril' as descricao
	  	   						union all
	  	   						select '5' as codigo, 'Maio' as descricao
	  	   						union all
	  	   						select '6' as codigo, 'Junho' as descricao
	  	   						union all
	  	   						select '7' as codigo, 'Julho' as descricao
	  	   						union all
	  	   						select '8' as codigo, 'Agosto' as descricao
	  	   						union all
	  	   						select '9' as codigo, 'Setembro' as descricao
	  	   						union all
	  	   						select '10' as codigo, 'Outubro' as descricao
	  	   						union all
	  	   						select '11' as codigo, 'Novembro' as descricao
	  	   						union all
	  	   						select '12' as codigo, 'Dezembro' as descricao
	  	   						";

                $db->monta_combo( 'pmes', $sql, 'S', 'Mês...','', '', '','', 'N', 'pmes', '', $pmes, 'Mês' );
                ?>
                /
                <?php
                $anoini = (int) 2014;
                $anofim = (int) date("Y");

                for($i=$anoini; $i<=$anofim; $i++){
                    $sqlano .= " select '$i' as codigo, '$i' as descricao ";
                    if($i != $anofim) $sqlano .= " union ";
                }
                $sqlano .= " order by 1 ";
                $db->monta_combo( 'pano', $sqlano, 'S', 'Ano...', '', '', '', '', 'N', 'pano', '', $pano, 'Ano' );

                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="3">
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarProcesso();"/>
                <input type="reset" name="vertodos" value="Ver todos" onclick="pesquisarProcesso();"/>
                <input type="button" name="btn_excel" value="Exportar p/ Excel" onclick="exportarXls();"/>
            </td>
        </tr>
    </table>
</form>


<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
    <tr>
        <td class="SubTituloEsquerda" colspan="2">
            <span class="adicionar" style="cursor:pointer"><img border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif">&nbsp;Novo Processo</span>
        </td>
    </tr>
</table>

<div id="div_lista_processo"><?php listarProcesso(); ?></div>