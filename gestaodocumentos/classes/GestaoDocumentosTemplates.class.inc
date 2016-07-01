<?php

/**
 * Primeiro passo pra separar a camada VIEW
 */
class GestaoDocumentosTemplates
{
	public function includesSistemaTabelasApoio()
	{
		?>
		<head>

		    <script type="text/javascript" src="/includes/funcoes.js"></script>
		    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

		    <script type="text/javascript">

		        $( document ).ready( function() {

		        });

		        function abrirPesquisa(opc){
		            if(opc == 'mais'){
		                //ABRE AS TR
		                $('.tr_orgao').css("display", "");
		                $('#tr_tipo_orgao').css("display", "");
		                $('#tr_botao').css("display", "");
		                //MUDAR A IMAGEM E O VALOR.
		                $('#sinal_mais').css("display", "none");
		                $('#sinal_menos').css("display", "");
		            }else{
		                //ESCONDE AS TR
		                $('.tr_orgao').css("display", "none");
		                $('#tr_tipo_orgao').css("display", "none");
		                $('#tr_botao').css("display", "none");
		                //MUDAR A IMAGEM E VALOR
		                $('#sinal_mais').css("display", "");
		                $('#sinal_menos').css("display", "none");
		            }
		        }

		        function buscaDados( pk ){
		            $.ajax({
		                type    : "POST",
		                url     : window.location,
		                data    : "requisicao=buscaDados&pk="+pk,
		                success: function(resp){
		                    var dados = $.parseJSON(resp);
		                	$.each( dados, function( index, value ){
		                		$('#'+index).val(value);
		                	});
		                }
		            });
		        }

		        function excluirDados( pk ){
		            var confirma = confirm("Deseja realmente excluir o Registro?");
		            if( confirma ){
		                $.ajax({
		                    type    : "POST",
		                    url     : window.location,
		                    data    : "requisicao=excluirDados&pk="+pk,
		                    asynchronous: false,
		                    success: function(resp){
		                        resp = pegaRetornoAjax('<resp>', '</resp>', resp, true);
		                        if( trim(resp) == 'OK' ){
		                            alert('Operação Realizada com sucesso!');
		                            window.location.href = window.location.href;
		                        }else{
		                            alert('Ocorreu um problema, não foi possivél realizar a operação.');
		                        }
		                    }
		                });
		            }
		        }

		        function limparDados( campos ){
		        	for (var i = campos.length - 1; i >= 0; i--) {
		        		$('#'+campos[i]).val('');
		        	};
		            return true;
		        }

		        function salvarDados( camposObrigatorios, labels ){
		            var erro;

		        	for (var i = camposObrigatorios.length - 1; i >= 0; i--) {
		        		var campo = $('#'+camposObrigatorios[i]);
		        		if( !campo.val() ){
		        			alert('O campo "'+labels[i]+'" é um campo obrigatório!');
			                campo.focus();
			                erro = 1;
			                return false;
		        		}
		        	};

		            if(!erro){
		                $('#requisicao').val('salvarDados');
		                $('#formulario').submit();
		            }
		        }

		        function pesquisar( param ){
		            if(trim(param) == 'fil'){
		                $('#tipo_fil').val('fil');
		                $('#formulario').submit();
		            }else{
		                $('#tipo_fil').val('');
		                $('#formulario').submit();
		            }
		        }

		    </script>

		</head> <?php
	}

	public function formCadastroTipoDocumento( TipoDocumento $TipoDocumento )
	{
		global $db;

		// ############################################################################################################

	    monta_titulo('Cadastrar Tipo Documento','Gest&atilde;o de Demandas SERES');
		?>
		<br>
	    <form name="formulario" id="formulario" action="" method="POST">
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <input type="hidden" name="requisicao" id="requisicao" value="" />
            <input type="hidden" name="tipo_fil" id="tipo_fil" value="" />
            <input type="hidden" name="tpdid" id="tpdid" value="" />
            <tr>
                <td colspan="2" class="subTituloCentro" style="text-align: left;">CADASTRAR TIPO DOCUMENTO</td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Descri&ccedil;&atilde;o:</td>
                <td>
                    <?php
                        echo campo_texto('tpddescricao', 'N', 'S', 'Descri&ccedil;&atilde;o', 56, 100, '', '', '', '', '', 'id="tpddescricao"', '', $tpddescricao);
                    ?>
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Status:</td>
                <td>
                    <?php
                    	$arrayStatus = array(array('codigo'=>'A','descricao'=>'Ativo'),array('codigo'=>'I','descricao'=>'Inativo'));
                    	echo $db->monta_combo('tpdstatus',$arrayStatus,'S',$titulo='Status','','','','','', 'tpdstatus', '', $tpdstatus);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDados( ['<?= implode('\',\'',$TipoDocumento->getObrigatorios()) ?>'], ['<?= implode('\',\'',$TipoDocumento->getLabels()) ?>'] );">
                    <input type="button" name="btnLimpar" value="Limpar Campos" id="btnLimpar" onclick="limparDados( ['<?= implode('\',\'',$TipoDocumento->getAtributos()) ?>'] );">
                </td>
            </tr>
        </table>

	    <br>

	    <table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">
	        <tr>
	            <td colspan="2" class="subTituloCentro" style="text-align: left;">
	                <img id="sinal_mais"  style="cursor: pointer; " src="../imagens/mais.gif" onclick="abrirPesquisa('mais');">
	                <img id="sinal_menos" style="display: none; cursor: pointer;" src="../imagens/menos.gif" onclick="abrirPesquisa('menos');">
	                &nbsp;&nbsp;
	                PESQUISAR POR TIPO DOCUMENTO
	            </td>
	        </tr>
	        <tr class="tr_orgao" style="display: none;">
	            <td class = "subtitulodireita" width="33%">Tipo Documento:</td>
	            <td>
	                <?PHP
	                    echo campo_texto('tpddescricao_pes', 'N', 'S', 'Tipo Documento', 56, 30, '', '', '', '', '', 'id="tpddescricao_pes"', '', $_REQUEST['tpddescricao_pes']);
	                ?>
	            </td>
	        </tr>
	        <tr class="tr_orgao" style="display: none;">
	            <td class = "subtitulodireita" width="33%">Status Tipo Documento:</td>
	            <td>
	                <?PHP
	                    echo campo_texto('tpdstatus_pes', 'N', 'S', 'Status Tipo Documento', 56, 30, '', '', '', '', '', 'id="tpdstatus_pes"', '', $_REQUEST['tpdstatus_pes']);
	                ?>
	            </td>
	        </tr>
	        <tr id="tr_botao" style="display: none;">
	            <td class="SubTituloDireita" colspan="2" style="text-align:center">
	                <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisar('fil');">
	                <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisar('tudo');">
	            </td>
	        </tr>
	    </table>

		</form>
		<br>
		<div id="div_tabela_busca_orgao" style="width:100%; overflow:auto;">
		    <?php $TipoDocumento->listaTipoDocumento(); ?>
		</div>
		<?php // ############################################################################################################
	}


	public function formCadastroTipoExpediente( TipoExpediente $TipoExpediente )
	{
		global $db;

		// ############################################################################################################

	    monta_titulo('Cadastrar Tipo Expediente','Gest&atilde;o de Demandas SERES');

		?>
		<br>
	    <form name="formulario" id="formulario" action="" method="POST">
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <input type="hidden" name="requisicao" id="requisicao" value="" />
            <input type="hidden" name="tipo_fil" id="tipo_fil" value="" />
            <input type="hidden" name="tpeid" id="tpeid"  value="" />
            <tr>
                <td colspan="2" class="subTituloCentro" style="text-align: left;">CADASTRAR TIPO EXPEDIENTE</td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Descri&ccedil;&atilde;o:</td>
                <td>
                    <?php
                        echo campo_texto('tpedsc', 'N', 'S', 'Descri&ccedil;&atilde;o', 56, 100, '', '', '', '', '', 'id="tpedsc"', '', $tpedsc);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDados( ['<?= implode('\',\'',$TipoExpediente->getObrigatorios()) ?>'], ['<?= implode('\',\'',$TipoExpediente->getLabels()) ?>'] );">
                    <input type="button" name="btnLimpar" value="Limpar Campos" id="btnLimpar" onclick="limparDados( ['<?= implode('\',\'',$TipoExpediente->getAtributos()) ?>'] );">
                </td>
            </tr>
        </table>

	    <br>

	    <table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">
	        <tr>
	            <td colspan="2" class="subTituloCentro" style="text-align: left;">
	                <img id="sinal_mais"  style="cursor: pointer; " src="../imagens/mais.gif" onclick="abrirPesquisa('mais');">
	                <img id="sinal_menos" style="display: none; cursor: pointer;" src="../imagens/menos.gif" onclick="abrirPesquisa('menos');">
	                &nbsp;&nbsp;
	                PESQUISAR POR TIPO EXPEDIENTE
	            </td>
	        </tr>
	        <tr class="tr_orgao" style="display: none;">
	            <td class = "subtitulodireita" width="33%">Tipo Expediente:</td>
	            <td>
	                <?PHP
	                    echo campo_texto('tpedsc_pes', 'N', 'S', 'Tipo Expediente', 56, 30, '', '', '', '', '', 'id="tpedsc_pes"', '', $_REQUEST['tpedsc_pes']);
	                ?>
	            </td>
	        </tr>
	        <tr id="tr_botao" style="display: none;">
	            <td class="SubTituloDireita" colspan="2" style="text-align:center">
	                <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisar('fil');">
	                <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisar('tudo');">
	            </td>
	        </tr>
	    </table>

		</form>
		<br>
		<div id="div_tabela_busca_orgao" style="width:100%; overflow:auto;">
			<?php $TipoExpediente->listaTipoExpediente(); ?>
		</div>
		<?php // ############################################################################################################
	}


	public function formCadastroTipoUnidade( TipoUnidade $TipoUnidade )
	{
		global $db;

		// ############################################################################################################

	    monta_titulo('Cadastrar Tipo Unidade','Gest&atilde;o de Demandas SERES');

		?>
		<br>
	    <form name="formulario" id="formulario" action="" method="POST">
        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
            <input type="hidden" name="requisicao" id="requisicao" value="" />
            <input type="hidden" name="tipo_fil" id="tipo_fil" value="" />
            <input type="hidden" name="tunid" id="tunid"  value="" />
            <tr>
                <td colspan="2" class="subTituloCentro" style="text-align: left;">CADASTRAR TIPO UNIDADE</td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Descri&ccedil;&atilde;o:</td>
                <td>
                    <?php
                        echo campo_texto('tundescricao', 'N', 'S', 'Descri&ccedil;&atilde;o', 56, 100, '', '', '', '', '', 'id="tundescricao"', '', $tundescricao);
                    ?>
                </td>
            </tr>
            <tr>
                <td class = "subtitulodireita">Profundidade:</td>
                <td>
                    <?php
                    	$arrayProfundidade = array(array('codigo'=>'1','descricao'=>'Diretoria'),array('codigo'=>'2','descricao'=>'SuperintendÃªncia'),array('codigo'=>'3','descricao'=>'Secretaria'));
                    	echo $db->monta_combo('tunprofundidade',$arrayProfundidade,'S',$titulo='Profundidade','','','','','', 'tunprofundidade', '', $tunprofundidade);
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                	<input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDados( ['<?= implode('\',\'',$TipoUnidade->getObrigatorios()) ?>'], ['<?= implode('\',\'',$TipoUnidade->getLabels()) ?>'] );">
                    <input type="button" name="btnLimpar" value="Limpar Campos" id="btnLimpar" onclick="limparDados( ['<?= implode('\',\'',$TipoUnidade->getAtributos()) ?>'] );">
                </td>
            </tr>
        </table>

	    <br>

	    <table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">
	        <tr>
	            <td colspan="2" class="subTituloCentro" style="text-align: left;">
	                <img id="sinal_mais"  style="cursor: pointer; " src="../imagens/mais.gif" onclick="abrirPesquisa('mais');">
	                <img id="sinal_menos" style="display: none; cursor: pointer;" src="../imagens/menos.gif" onclick="abrirPesquisa('menos');">
	                &nbsp;&nbsp;
	                PESQUISAR POR TIPO UNIDADE
	            </td>
	        </tr>
	        <tr class="tr_orgao" style="display: none;">
	            <td class = "subtitulodireita" width="33%">Tipo Unidade:</td>
	            <td>
	                <?PHP
	                    echo campo_texto('tundescricao_pes', 'N', 'S', 'Tipo Unidade', 56, 30, '', '', '', '', '', 'id="tundescricao_pes"', '', $_REQUEST['tundescricao_pes']);
	                ?>
	            </td>
	        </tr>
	        <tr class="tr_orgao" style="display: none;">
	            <td class = "subtitulodireita" width="33%">Profundidade:</td>
	            <td>
	                <?php
                    	echo $db->monta_combo('tunprofundidade_pes',$arrayProfundidade,'S',$titulo='Profundidade','','','','','', 'tunprofundidade_pes', '', $_REQUEST['tunprofundidade_pes']);
                    ?>
	            </td>
	        </tr>
	        <tr id="tr_botao" style="display: none;">
	            <td class="SubTituloDireita" colspan="2" style="text-align:center">
	                <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisar('fil');">
	                <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisar('tudo');">
	            </td>
	        </tr>
	    </table>

		</form>
		<br>
		<div id="div_tabela_busca_orgao" style="width:100%; overflow:auto;">
		    <?php $TipoUnidade->listaTipoUnidade(); ?>
		</div>
		<?php // ############################################################################################################
	}


	public function formCadastroTema( Tema $Tema ){
            global $db;

            monta_titulo('Cadastrar Assunto','Gest&atilde;o de Demandas SERES');

        ?>
            <br>

            <form name="formulario" id="formulario" action="" method="POST">
                <table  bgcolor="#f5f5f5" align="center" class="tabela" >
                    <input type="hidden" name="requisicao" id="requisicao" value="" />
                    <input type="hidden" name="tipo_fil" id="tipo_fil" value="" />
                    <input type="hidden" name="temid" id="temid"  value="" />
                    <tr>
                        <td colspan="2" class="subTituloCentro" style="text-align: left;">CADASTRAR ASSUNTO</td>
                    </tr>
                    <tr>
                        <td class = "subtitulodireita">Descrição:</td>
                        <td>
                            <?php
                                echo campo_texto('temdescricao', 'N', 'S', 'Descri&ccedil;&atilde;o', 56, 100, '', '', '', '', '', 'id="temdescricao"', '', $temdescricao);
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class = "subtitulodireita">Status:</td>
                        <td>
                            <?php
                                $temstatus = array(array('codigo'=>'A','descricao'=>'Ativo'),array('codigo'=>'I','descricao'=>'Inativo'));
                                echo $db->monta_combo('temstatus',$temstatus,'S',$titulo='Status','','','','','', 'temstatus', '', $temstatus);
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" colspan="2" style="text-align:center">
                            <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDados( ['<?= implode('\',\'',$Tema->getObrigatorios()) ?>'], ['<?= implode('\',\'',$Tema->getLabels()) ?>'] );">
                            <input type="button" name="btnLimpar" value="Limpar Campos" id="btnLimpar" onclick="limparDados( ['<?= implode('\',\'',$Tema->getAtributos()) ?>'] );">
                        </td>
                    </tr>
                </table>

                <br>

                <table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">
                    <tr>
                        <td colspan="2" class="subTituloCentro" style="text-align: left;">
                            <img id="sinal_mais"  style="cursor: pointer; " src="../imagens/mais.gif" onclick="abrirPesquisa('mais');">
                            <img id="sinal_menos" style="display: none; cursor: pointer;" src="../imagens/menos.gif" onclick="abrirPesquisa('menos');">
                            &nbsp;&nbsp;
                            PESQUISAR POR ASSUNTO
                        </td>
                    </tr>
                    <tr class="tr_orgao" style="display: none;">
                        <td class = "subtitulodireita" width="33%">Assunto:</td>
                        <td>
                            <?PHP
                                echo campo_texto('temdescricao_pes', 'N', 'S', 'Tema', 56, 30, '', '', '', '', '', 'id="temdescricao_pes"', '', $_REQUEST['temdescricao_pes']);
                            ?>
                        </td>
                    </tr>
                    <tr class="tr_orgao" style="display: none;">
                        <td class = "subtitulodireita" width="33%">Status:</td>
                        <td>
                            <?php
                                echo $db->monta_combo('temstatus_pes',$temstatus,'S',$titulo='Status','','','','','', 'temstatus_pes', '', $_REQUEST['temstatus_pes']);
                            ?>
                        </td>
                    </tr>
                    <tr id="tr_botao" style="display: none;">
                        <td class="SubTituloDireita" colspan="2" style="text-align:center">
                            <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisar('fil');">
                            <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisar('tudo');">
                        </td>
                    </tr>
                </table>

            </form>

            <br>
            <div id="div_tabela_busca_orgao" style="width:100%; overflow:auto;">
                <?php $Tema->listaTemas(); ?>
            </div>

<?php // ############################################################################################################
	}


	public function formCadastroTipoModalidade( TipoModalidade $TipoModalidade )
	{
		global $db;

		// ############################################################################################################

	    ?>
		<br>
	    <form name="formulario" id="formulario" action="" method="POST">
	        <table  bgcolor="#f5f5f5" align="center" class="tabela" >
	            <input type="hidden" name="requisicao" id="requisicao" value="" />
	            <input type="hidden" name="tipo_fil" id="tipo_fil" value="" />
	            <input type="hidden" name="tmdid" id="tmdid" value="" />
	            <tr>
	                <td colspan="2" class="subTituloCentro" style="text-align: left;">CADASTRAR TIPO MODALIDADE</td>
	            </tr>
	            <tr>
	                <td class = "subtitulodireita">Descri&ccedil;&atilde;o:</td>
	                <td>
	                    <?PHP
	                        echo campo_texto('tmddescricao', 'N', 'S', 'Descri&ccedil;&atilde;o', 56, 100, '', '', '', '', '', 'id="tmddescricao"', '', $tmddescricao);
	                    ?>
	                </td>
	            </tr>
	            <tr>
	                <td class="SubTituloDireita" colspan="2" style="text-align:center">
	                    <input type="button" name="btnSalvar" value="Salvar" id="btnSalvar" onclick="salvarDados( ['<?= implode('\',\'',$TipoModalidade->getObrigatorios()) ?>'], ['<?= implode('\',\'',$TipoModalidade->getLabels()) ?>'] );">
                    	<input type="button" name="btnLimpar" value="Limpar Campos" id="btnLimpar" onclick="limparDados( ['<?= implode('\',\'',$TipoModalidade->getAtributos()) ?>'] );">
	                </td>
	            </tr>
	        </table>

	        <br>

	        <table  bgcolor="#f5f5f5" align="center" class="tabela" border="0">
	            <tr>
	                <td colspan="2" class="subTituloCentro" style="text-align: left;">
	                    <img id="sinal_mais"  style="cursor: pointer; " src="../imagens/mais.gif" onclick="abrirPesquisa('mais');">
	                    <img id="sinal_menos" style="display: none; cursor: pointer;" src="../imagens/menos.gif" onclick="abrirPesquisa('menos');">
	                    &nbsp;&nbsp;
	                    PESQUISAR POR TIPO MODALIDADE
	                </td>
	            </tr>
	            <tr class="tr_orgao" style="display: none;">
	                <td class = "subtitulodireita" width="33%">Tipo Modalidade:</td>
	                <td>
	                    <?PHP
	                        echo campo_texto('tmddescricao_pes', 'N', 'S', 'Tipo Modalidade', 56, 30, '', '', '', '', '', 'id="tmddescricao_pes"', '', $_REQUEST['tmddescricao_pes']);
	                    ?>
	                </td>
	            </tr>
	            <tr id="tr_botao" style="display: none;">
	                <td class="SubTituloDireita" colspan="2" style="text-align:center">
	                    <input type="button" name="buscar" value="Buscar" id="buscar" onclick="pesquisar('fil');">
	                    <input type="button" name="ver_todos" value="Ver Todos" id="ver_todos" onclick="pesquisar('tudo');">
	                </td>
	            </tr>
	        </table>

	    </form>
	    <br>
	    <div id="div_tabela_busca_orgao" style="width:100%; overflow:auto;">
	        <?php $TipoModalidade->listaTipoModalidade(); ?>
	    </div>
		<?php // ############################################################################################################
	}

}