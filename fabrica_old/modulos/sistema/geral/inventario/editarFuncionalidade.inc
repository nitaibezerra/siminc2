<?php 
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";


//Monta o Título da página abaixo da aba
monta_titulo("INVENTÁRIO", "<b>Editar Funcionalidade</b>");


$coInventario       = (int) $_REQUEST['co_inventario'];
$coFuncionalidade   = (int) $_REQUEST['co_funcionalidade'];

if(empty($coInventario) || empty($coFuncionalidade)){
?>   
    <script type="text/javascript">
        alert('Erro ao acessar a página, por favor tente novamente');
        history.go(-1);
    </script>
<?php    
    exit;
}

$sqlFuncionalidade  = "SELECT inv.co_inventario, to_char(inv.dt_cadastro,'DD/MM/YYYY') as dt_cadastro, sid.sidid,
                        sid.siddescricao, usu.usunome, usu.usucpf, func.co_agrupador, func.co_funcionalidade, func.ds_funcionalidade 	                 
                        , func.co_tipo_funcionalidade, func.qt_alr_rlr, func.qt_td, func.qtd_pf, tp_complexidade
                        , func.ds_alr_rlr, func.ds_td
                    FROM inventario.tb_simec_inventario inv
                    INNER JOIN demandas.sistemadetalhe sid
                        ON inv.sidid = sid.sidid   
                    INNER JOIN seguranca.usuario usu
                        ON  inv.usucpf = usu.usucpf
                    INNER JOIN inventario.tb_simec_funcionalidade func
                        ON inv.co_inventario = func.co_inventario
                    WHERE inv.co_inventario = {$coInventario}
                    AND co_funcionalidade = {$coFuncionalidade}";
     

$dadosFuncionalidade = $db->pegaLinha( $sqlFuncionalidade );

$sidid                  = $dadosFuncionalidade['sidid'];
$sistema                = $dadosFuncionalidade['siddescricao'];
$coAgrupador            = $dadosFuncionalidade['co_agrupador'];
$coFuncionalidade       = $dadosFuncionalidade['co_funcionalidade'];
$coInventario           = $dadosFuncionalidade['co_inventario'];
$coTipoFuncionalidade   = $dadosFuncionalidade['co_tipo_funcionalidade'];
$ds_funcionalidade      = $dadosFuncionalidade['ds_funcionalidade'];
$qtdalrrlr              = $dadosFuncionalidade['qt_alr_rlr'];
$qtdtd                  = $dadosFuncionalidade['qt_td'];
$qtd_pf                 = $dadosFuncionalidade['qtd_pf'];
$tipoComplexidade       = $dadosFuncionalidade['tp_complexidade'];
$descricaoALRRLR        = $dadosFuncionalidade['ds_alr_rlr'];
$descricaoTD            = $dadosFuncionalidade['ds_td'];

$tipo_complexidade       = '';

switch( $tipoComplexidade ) {
    case 'A': 
        $tipo_complexidade = 'Alta';
        break;
    case 'M': 
        $tipo_complexidade = 'Média';
        break;
    case 'B': 
        $tipo_complexidade = 'Baixa';
        break;
}


?>
<link rel='stylesheet' type='text/css' href='./css/jquery-ui-1.8.16.custom.css'/>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<link href="./css/fabrica.css" type="text/css" rel="stylesheet" />

<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="./js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="./js/jquery.validate.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="./js/inventario/tipo-funcao.js"></script>
<script type="text/javascript" src="./js/inventario/tipo-funcao-ali.js"></script>
<script type="text/javascript" src="./js/inventario/tipo-funcao-aie.js"></script>
<script type="text/javascript" src="./js/inventario/tipo-funcao-ee.js"></script>
<script type="text/javascript" src="./js/inventario/tipo-funcao-se.js"></script>
<script type="text/javascript" src="./js/inventario/tipo-funcao-ce.js"></script>
<script type="text/javascript" src="./js/inventario.js"></script>
<script>
function obrigatorio()
{
	$obrigatorio = " <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />";
	return $obrigatorio;
}

function btnVoltar(){
	history.go(-1);
}

</script>

<form id="form-inserirInventario" name="form-inserirInventario" method="post" action="#" >
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubtituloDireita" width="25%">Sistema:</td>
            <td>
                <? echo campo_texto('sistema', 'N', 'N', "Sistema", '50', '', '','', '', '', 0, '', '', '', '', ''); ?>
            </td>        
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Agrupador de Funcionalidades:</td>
            <td>
                <?php
                $sql = "SELECT co_agrupador as codigo,
								no_agrupador as descricao
                        FROM inventario.tb_simec_agrupador_funcionalidade
						WHERE st_agrupador_funcionalidade = 'A'
						AND sidid={$sidid}
                        ORDER BY no_agrupador";
                echo $db->monta_combo('co_agrupador', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'co_agrupador', '', $coAgrupador );
                ?>
                &nbsp;
                <a  onclick="novoAgrupador()" style="cursor: pointer"><b><u>Novo</u></b></a>
            </td>
        </tr>
        <tr id="rowNovoAgrupador" style="display: none;">
            <td class="SubtituloDireita" width="25%"></td>
            <td>
                <?php echo campo_texto("no_agrupador",'N','S','Nome Agrupador Funcionalidade',"50","50","","","","","","","") ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%">Funcionalidade:</td>
            <td>
                <?php echo campo_texto("ds_funcionalidade",'S','S','Descrição',"50","50","","","","","","","") ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Tipo:</td>
            <td>
                <?php
                
                $sql = "SELECT co_tipo_funcionalidade as codigo,
								ds_tipo_funcionalidade as descricao
                        FROM inventario.tb_simec_tipo_funcionalidade
						WHERE st_tipo_funcionalidade = 'A'
                        ORDER BY ds_tipo_funcionalidade";
                echo $db->monta_combo('co_tipo_funcionalidade', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'co_tipo_funcionalidade', '', $coTipoFuncionalidade);
                
                ?>
        </tr>
        <tr>
            <td class="SubtituloDireita">Qtd ALR/RLR:</td>
            <td>
                <?php echo campo_texto("qtdalrrlr",'S','S','Qtd ALR/RLR',"5","","[#]","","","","","","") ?>
                <img src="/imagens/gif_inclui.gif" id="btnAdicionarDSALRRLR" alt="Descrição ALR/RLR" title="Descrição ALR/RLR" />
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>
		<tr>
            <td class="SubtituloDireita">Qtd TD:</td>
            <td>
                <?php echo campo_texto("qtdtd",'S','S','Qtd TD',"5","","[#]","","","","","","") ?>
                <img src="/imagens/gif_inclui.gif" id="btnAdicionarTD" alt="Descrição TD" title="Descrição TD" />
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>
		<tr>
            <td class="SubtituloDireita">Complexidade:</td>
            <td>
                <?php echo campo_texto("tipo_complexidade",'N','S','Qtd PF',"10","6","","","","","","","") ?>
                <input type="hidden" name="tp_complexidade" id="tp_complexidade" value="<?php echo $tipoComplexidade ?>" />
            </td>
        </tr>
		<tr>
            <td class="SubtituloDireita">Qtd PF:</td>
            <td><?php echo campo_texto("qtd_pf",'N','S','Qtd PF',"10","6","","","","","","","") ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita" width="25%"></td>
            <td>
				<input type="button" id="salvar" value="Salvar"  />
            	<input type="button" id="voltar" value="Voltar"  onclick="btnVoltar()" />
            </td>
        </tr>
    </table>

    
    <input type="hidden" name="ds_td" id="ds_td" value="<?php echo $descricaoALRRLR; ?>" />
    <input type="hidden" name="ds_alr_rlr" id="ds_alr_rlr" value="<?php echo $descricaoTD; ?>" />
    <input type="hidden" name="sidid" id="sidid" value="<?php echo $sidid ?>" />
    <input type="hidden" name="co_funcionalidade" id="co_funcionalidade" value="<?php echo $coFuncionalidade; ?>" />
    <input type="hidden" name="co_inventario" id="co_inventario" value="<?php echo $coInventario; ?>" />
    
</form>

<div id="modalDescricaoALRRLR" style="display: none;">
    <p>
        <span class="campos">
            <textarea id="descricao_alr_rlr" name="descricao_alr_rlr" cols="50" maxlength="300" rows="5" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this ); " style="width:50ex; resize:none;"  class="obrigatorio txareanormal"><?php echo $descricaoALRRLR; ?></textarea>
        </span>
    </p>
</div>

<div id="modalDescricaoTD" style="display: none;">
    <p>
        <span class="campos">
            <textarea id="descricao_td" name="descricao_td" cols="50" maxlength="300" rows="5" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this ); " style="width:50ex; resize:none;"  class="obrigatorio txareanormal"><?php echo $descricaoTD; ?></textarea>
        </span>
    </p>
</div>
<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script type="text/javascript">
    $(document).ready(function(){
        Inventario.iniciarModal();

        $('#co_agrupador').focus();

        $('input[name="qtdalrrlr"]').blur( Inventario.calcularComplexidadeHandler );
        $('input[name="qtdtd"]').blur( Inventario.calcularComplexidadeHandler );
        $('#co_tipo_funcionalidade').change( Inventario.calcularComplexidadeHandler );
        
        $('#btnAdicionarDSALRRLR').click(function(){ $('#modalDescricaoALRRLR').dialog('open');});
        $('#btnAdicionarTD').click(function(){  $('#modalDescricaoTD').dialog('open');});
        
        $('#co_agrupador').change(function(){
            $('#rowNovoAgrupador').hide();
            $('input[name="no_agrupador"]').rules('remove');
            $('#co_agrupador').rules('add', { required : true , messages : { required : 'Campo Obrigatório' } } );
        });
        
        $('#salvar').click(Inventario.alterarFuncionalidadeHandler);

        
        $(document).keypress(function(event) {
	       	 if (event.which == 13) {
				if( $('#modalDescricaoTD').dialog('isOpen') || $('#modalDescricaoALRRLR').dialog('isOpen')){
					//$('#modalDescricaoTD').dialog('close');
					//$('#modalDescricaoALRRLR').dialog('close');
				}else{
					$('#salvar').trigger("click");
				}
	       	 }
		});
        
         $("#form-inserirInventario").validate({
             //Define as regras dos campos
             rules:{
                     sidid:{
                             required: true
                     },
                     co_agupador:{
                             required: true
                     },
                     ds_funcionalidade:{
                             required: true
                     },
                     co_tipo_funcionalidade:{
                         required: true
					   },
                     qtdalrrlr:{
                         required: true
                 	   },
                     qtdtd:{
                         required: true
                 	   }
             },
             //Define as mensagesn de alerta
             messages:{
						sidid                  	 :"Campo Obrigatório",
						co_agupador              :"Campo Obrigatório",
						ds_funcionalidade        :"Campo Obrigatório",
						co_tipo_funcionalidade   :"Campo Obrigatório",
						qtdalrrlr                :"Campo Obrigatório",
						qtdtd                    :"Campo Obrigatório"
             }
      });
  });
    
    
    
    function novoAgrupador()
    {
        $('#rowNovoAgrupador').toggle('fast', function(){
            
            if( $(this).is(':visible') ) {
                $('input[name="no_agrupador"]').rules('add', { required : true , messages : { required : 'Campo Obrigatório' } } );
                $('#co_agrupador').rules('remove');
                $('#co_agrupador option[value=""]').attr('selected', 'selected');
            }else {
                $('input[name="no_agrupador"]').rules('remove');
                $('#co_agrupador').rules('add', { required : true , messages : { required : 'Campo Obrigatório' } } );
                
            }
            
        });
    }
    
</script>