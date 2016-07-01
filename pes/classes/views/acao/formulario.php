<?php
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

global $db;

if($this->permission < 3)
    $save = 'S';
else 
    $save = 'N';

?>
<?php if(empty($this->dataForm['acacodigo'])): ?>
<style>
    .container_form_save{
        display: none;
    }
</style>
<?php endif ?>
<?php if($this->permission < 3): ?> 
<form method="POST"  name="formulario" id="form_save" action="<?php // echo '/pes/pes.php?modulo=principal/acao/cadastro&acao=A'    ?>">
    <?php endif ?>
    <input type='hidden' name="action" value="salvar">
    <input type='hidden' name="acacodigo" value="<?php echo $this->dataForm['acacodigo'] ?>">
    <center>
        <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width: 100%;">
            <tr>
                <?php
                    $sql = "select tidnome as descricao, tidcodigo as codigo
                            from  pes.pestipodespesa
                            where tidcodigo not in (" . K_DESPESA_DIARIAS . ", " . K_DESPESA_PASSAGENS . ", " . K_DESPESA_COLETA_SELETIVA . ", " . K_DESPESA_GENERICA . ")
                            order by tidordem desc, descricao";
                ?>

                <td align='right' class="SubTituloDireita">Despesa:</td>
                <td><?php echo $db->monta_combo( "tidcodigo", $sql, 'S', "Selecione", 'listar', '', '', '', 'S', 'tidcodigo', '', $this->dataForm['tidcodigo']); ?></td>
            </tr>
            <tr class="container_form_save">
                <td align='right' class="SubTituloDireita">Descrição da ação:</td>
                <td>
                    <div style="float:left"><?php echo campo_textarea( 'acadescricaoacao', 'S', $save, '', 64, 05, 4000, '', '', '', '', 'id="acadescricaoacao"', $this->dataForm['acadescricaoacao'] ); ?></div>
                    <div style="float:left"><img id="img_sugestao" border="0" title="Sugestões de Ação" src="../imagens/busca.gif"></div>
                    <div style="clear: both;"></div>
                </td>
            </tr>
            <tr class="container_form_save">
                <td align='right' class="SubTituloDireita">Responsável pela ação:</td>
                <td><?php echo campo_texto( 'acanomeresponsavel', 'S', $save, '', 49, 60, '', '', '', '', '', 'id="acanomeresponsavel"', '', $this->dataForm['acanomeresponsavel'] ); ?></td>
            </tr>
            <tr class="container_form_save">
                <td align='right' class="SubTituloDireita">Observações:</td>
                <td><?php echo campo_textarea( 'acaobservacao', 'N', $save, '', 64, 05, 4000, '', '', '', '', 'id="acaobservacao"', $this->dataForm['acaobservacao'] ); ?></td>
            </tr>
            <tr class="container_form_save">
                <td align='right' class="SubTituloDireita">Data de início prevista:</td>
                <td><?= campo_texto( 'acadataprevisaoinicio', 'S', $save, '', 10, 10, '##/##/####', '', '', '', '', 'id="acadataprevisaoinicio"', '', $this->dataForm['acadataprevisaoinicio'] ); ?></td>
            </tr>
            <tr class="container_form_save">
                <td align='right' class="SubTituloDireita">Data de fim prevista:</td>
                <td><?= campo_texto( 'acadataprevisaofim', 'S', $save, '', 10, 10, '##/##/####', '', '', '', '', 'id="acadataprevisaofim"', '', $this->dataForm['acadataprevisaofim'] ); ?></td>
            </tr>
            <tr class="container_form_save container_botao">
                <td colspan="2">
            <?php if($this->permission < 3): ?> 
                    <input type="button" name="btinserir" value="Salvar" onclick="javascript:salvar();" class="botao">
            <?php endif ?>
                    <input type="button" name="btcancela" value="Cancelar" onclick="javascript:esconderFormularioSalvar();" class="botao">
                </td>
            </tr>
            <?php if($this->permission < 3): ?>
            <tr class="container_botao">
                <td colspan="2" class="container_button_save" <?php if(!empty($this->dataForm['acacodigo'])) echo 'style="display: none;"' ?>>
                    <input type="button" value="Inserir" onclick="javascript:exibirFormularioSalvar();"/>
                    <br />
                </td>
            </tr>
            <?php endif ?>
        </table>
    </center>
    <br><br>
    <?php if($this->permission < 3): ?> 
</form>
<?php endif ?>

<div id="dialog-sugestao" title="Sugestões de Descrição de Ação"></div>

<script language="javascript" type="text/javascript">
    
    $.datepicker.regional[ 'pt-BR' ];
    $( "#acadataprevisaoinicio" ).datepicker();
    $( "#acadataprevisaofim" ).datepicker();
    
    
    jQuery(function(){
        jQuery('#img_sugestao').click(function(){
                
                $("#dialog-sugestao").remove();
                $('body').append('<div id="dialog-sugestao"></div>');
                
        	jQuery("#dialog-sugestao").load('pes.php?modulo=principal/planoacao/cadastro&acao=A&action=sugestao&tidcodigo=' + $('#tidcodigo').val());
        	jQuery("#dialog-sugestao").dialog({
                modal: true,
                width:  600,
                height: 500,
                buttons: {
                    Fechar: function() {
        		        $(this).dialog('close');
                    }
                }
            });
        });

        jQuery('#tidcodigo').change(function(){
            jQuery('#acadescricaoacao').val('');
        });
    });
                        /**
                         * listar
                         */
                        function listar( id ) {
                            if ( id ) {
                                var url = 'pes.php?modulo=principal/planoacao/cadastro&acao=A';
                                var dataForm = { action: 'listarAcao', tidcodigo: id };

                                $.post( url, dataForm, function( data ) {
                                    $( '.container_list' ).hide().html( data ).fadeIn( 'slow' );
                                } );
                            } else {
                                $( '.container_list' ).fadeOut( 'slow' );
                            }
                        }

                        /**
                         * formulario
                         */
                        function exibirFormularioSalvar() {
                            $( '.container_button_save' ).hide();
                            $( '.container_form_save' ).fadeIn( 'slow' );
                        }

                        /**
                         * esconderFormularioSalvar
                         */
                        function esconderFormularioSalvar( ) {
                            $( '#acacodigo' ).val( '' );
                            $( '#acadescricaoacao' ).val( '' );
                            $( '#acanomeresponsavel' ).val( '' );
                            $( '#acaobservacao' ).val( '' );
                            $( '#acadataprevisaoinicio' ).val( '' );
                            $( '#acadataprevisaofim' ).val( '' );

                            $( '.container_form_save' ).hide( );
                            $( '.container_button_save' ).fadeIn( 'slow' );
                        }

                        /**
                         *
                         */
                        function salvar()
                        {
                            var tidcodigo = $( '#tidcodigo' );
                            var acadescricaoacao = $( '#acadescricaoacao' );
                            var acanomeresponsavel = $( '#acanomeresponsavel' );
                            var acaobservacao = $( '#acaobservacao' );
                            var acadataprevisaoinicio = $( '#acadataprevisaoinicio' );
                            var acadataprevisaofim = $( '#acadataprevisaofim' );

                            if ( tidcodigo.val() == "" ) {
                                msg( tidcodigo, 'O campo "Despesa" é necessário!' );
                                return false;
                            }
                            if ( acadescricaoacao.val() == "" ) {
                                msg( acadescricaoacao, 'O campo "Descrição da ação" é necessário!' );
                                return false;
                            }
                            if ( acanomeresponsavel.val() == "" ) {
                                msg( acanomeresponsavel, 'O campo "Responsável pela ação" é necessário!' );
                                return false;
                            }
                            if ( acadataprevisaoinicio.val() == "" ) {
                                msg( acadataprevisaoinicio, 'O campo "Data de início prevista" é necessário!' );
                                return false;
                            }
                            if ( acadataprevisaofim.val() == "" ) {
                                msg( acadataprevisaofim, 'O campo "Data de fim prevista" é necessário!' );
                                return false;
                            }

                            saveSubmitAjax();
                        }

                        /**
                         * returnSaveSucess(){
                         */
                        function returnSaveSucess() {
                            esconderFormularioSalvar();
                            listar( $( '#tidcodigo' ).val() );
                        }
</script>