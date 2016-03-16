<?php 
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

//Monta o Título da página abaixo da aba
monta_titulo("INVENTÁRIO", "<b>Inserir Inventário</b>");

//Montando Lista de Sistemas
$sistemaRepositorio = new SistemaRepositorio();
$sistemasAtivos     = $sistemaRepositorio->recuperaNaoInventariados();

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

<script type="text/javascript">
function obrigatorio(){
	$obrigatorio = " <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />";
	return $obrigatorio;
}

</script>
<form id="form-inserirInventario" name="form-inserirInventario" method="post" action="#" >
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubtituloDireita" width="25%">Sistema:</td>
            <td>
                <select id="sidid" class="campoEstilo" name="sidid" style="width: auto;">
                    <option id="selecione" value="">Selecione</option>
					<?php foreach ( $sistemasAtivos as $sa ) { ?>
                        <option value="<?php echo $sa->getId(); ?>" ><?php echo $sa->getDescricao(); ?></option>
                    <?php } ?>
                </select><?php echo obrigatorio(); ?>
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
						AND sidid=0
                        ORDER BY no_agrupador";
                echo $db->monta_combo('co_agrupador', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'co_agrupador', '', $_REQUEST['co_agrupador']);
                ?>
                &nbsp;
                <a  onclick="novoAgrupador()" style="cursor: pointer"><b><u>Novo</u></b></a>
            </td>
        </tr>
        <tr id="rowNovoAgrupador" style="display: none;">
            <td class="SubtituloDireita" width="25%"></td>
            <td>
                <?php echo campo_texto("no_agrupador",'S','S','Nome Agrupador Funcionalidade',"50","50","","","","","","","") ?>
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
                echo $db->monta_combo('co_tipo_funcionalidade', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'co_tipo_funcionalidade', '', $_REQUEST['co_tipo_funcionalidade']);
                
                ?>
        </tr>
        <tr>
            <td class="SubtituloDireita">Qtd ALR/RLR:</td>
            <td>
                <?php echo campo_texto("qtdalrrlr",'S','S','Qtd ALR/RLR',"5","2","[#]","","","","","","") ?>
                <img src="/imagens/gif_inclui.gif" id="btnAdicionarDSALRRLR" class="obrigatorio" alt="Descrição ALR/RLR" title="Descrição ALR/RLR"  />
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>
		<tr>
            <td class="SubtituloDireita">Qtd TD:</td>
            <td>
                <?php echo campo_texto("qtdtd",'S','S','Qtd TD',"5","3","[#]","","","","","","") ?>
                <img src="/imagens/gif_inclui.gif" id="btnAdicionarTD" class="obrigatorio" alt="Descrição TD" title="Descrição TD" />
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
            </td>
        </tr>
		<tr>
            <td class="SubtituloDireita">Complexidade:</td>
            <td>
                <?php echo campo_texto("tipo_complexidade",'N','S','Complexidade',"10","6","","","","","","","") ?>
                <input type="hidden" name="tp_complexidade" id="tp_complexidade" value="A" />
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
            	<input type="reset" id="reset-limparInventario" value="Limpar Campos"  />

            </td>
        </tr>
    </table>

    <input type="hidden" name="ds_td" id="ds_td" value="" />
    <input type="hidden" name="ds_alr_rlr" id="ds_alr_rlr" value="" />
    
</form>

<div id="modalDescricaoALRRLR" style="display: none;">
    <p>
        <span class="campos">
            <textarea id="descricao_alr_rlr" name="descricao_alr_rlr" cols="50" maxlength="300" rows="5" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this ); " style="width:50ex; resize:none; "  class="txareanormal"></textarea>
        </span>
    </p>
</div>

<div id="modalDescricaoTD" style="display: none;">
    <p>
        <span class="campos">
            <textarea id="descricao_td" name="descricao_td" cols="50" maxlength="300" rows="5" onmouseover="MouseOver( this );" onfocus="MouseClick( this );" onmouseout="MouseOut( this );" onblur="MouseBlur( this ); " style="width:50ex; resize:none;" class="txareanormal"></textarea>
        </span>
    </p>
</div>
<div id="dialogAjax" >
    <img src="../imagens/wait.gif" alt="ajax"/>Aguarde! Carregando Dados...
</div>

<script type="text/javascript">
    $(document).ready(function(){

        Inventario.iniciarModal();
        
        $('#sidid').focus();
        //$('#sidid').change( Inventario.listarAgrupadores );
        
        $('input[name="qtdalrrlr"]').blur( Inventario.calcularComplexidadeHandler );
        $('input[name="qtdtd"]').blur( Inventario.calcularComplexidadeHandler );
        $('#co_tipo_funcionalidade').change( Inventario.calcularComplexidadeHandler );
        
        $('#btnAdicionarDSALRRLR').click(function(){ $('#modalDescricaoALRRLR').dialog('open');});
        $('#btnAdicionarTD').click(function(){  $('#modalDescricaoTD').dialog('open');});
        
        $('#co_agrupador').change(novoAgrupador2);
        
        $('#salvar').click(Inventario.salvarInventarioHandler);

        /*$('#form-inserirInventario').keypress(function(event) {
        	 if (event.which == 13) {
        	   $('#salvar').trigger("click");
        	 }
		});*/

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
                       sidid:{required: true},
                       co_agrupador:{required: true},
                       ds_funcionalidade:{required: true},
                       co_tipo_funcionalidade:{required: true},
                       qtdalrrlr:{required: true},
                       qtdtd:{required: true}
               },
               //Define as mensagesn de alerta
               messages:{
						sidid                  	 :"Campo Obrigatório",
						co_agrupador             :"Campo Obrigatório",
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

    function novoAgrupador2(){
		$('input[name="no_agrupador"]').rules('add', { required : true , messages : { required : 'Campo Obrigatório' } } );
		$('#co_agrupador').rules('remove');
		$('#rowNovoAgrupador').hide();

    }
    
</script>