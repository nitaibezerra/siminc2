<?php
require_once '_funcoes_esplanada_sustentavel.php';

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']($_REQUEST);
    die();
}

include APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';

$unicod = $_REQUEST['unicod'];
$unidade_decricao = $_REQUEST['descricao'];

// Se tiver dados post de enviar dados ao web service, comeca a transacao.
if (isset($_POST['boolean_enviar_dados'])) {
    include 'soap_sispes_planejamento.class.inc';
    
    // Chamando class do web service do SisPES para consultar qual o codigo da entidade de acordo com a uo selecionada para esta tela.
    $clsSoapSispesPlanejamento = new soapSispesPlanejamento();
    $result = $clsSoapSispesPlanejamento->consultarEntidadesDoOrgao();
    
    // Dados ficticios, simulando o retorno do webservice, para testar localmente.
    // MOCK  - Servico - consultarEntidadesDoOrgao
//    $result = new stdClass();
//    $result->return->codigoOrgao = 2600;
//    $result->return->mensagem = 'SUCESSO';
//    $result->return->nomeOrgao = 'Ministério da Educação';
//    $result->return->uos[0]->codigo =  26101;
//    $result->return->uos[0]->entidades->codigo =  251;
//    $result->return->uos[0]->entidades->nome =  'MEC';
//    $result->return->uos[0]->nome =  'Ministério da Educação';
//    $result->return->uos[1]->codigo =  26104;
//    $result->return->uos[1]->entidades =  '';
//    $result->return->uos[1]->nome =  'Instituto Nacional de Educação de Surdos';
//    $result->return->uos[2]->codigo =  26105;
//    $result->return->uos[2]->entidades =  '';
//    $result->return->uos[2]->nome =  'Instituto Benjamin Constant';
//    $result->return->uos[3]->codigo =  26231;
//    $result->return->uos[3]->entidades->codigo =  351;
//    $result->return->uos[3]->entidades->nome =  'Hahaha';
//    $result->return->uos[3]->nome =  'Instituto Benjamin Constant';
    
    // Pegando a entidade no WebService SisPES de acordo com a UO.
    foreach ($result->return->uos as $uo) {
        if (is_array($uo->entidades)) {
            $coEntidade = $uo->entidades[0]->codigo;
        } else {
            if(isset($uo->entidades->codigo) && $uo->codigo == $unicod)
            $coEntidade = $uo->entidades->codigo;
        }
    }
    
    // Se tiver entidade ele comeca o processo de salvar as despesas.
    if(isset($coEntidade)){
        $sql = "
			Select ds.dpsid,
				ds.dpsdescricao,
                ds.dpsidsispes,
				
				Case When lsa.lsavalorempenhado > 1 
					Then trim(to_char(lsa.lsavalorempenhado, '999G999G999G999G999D99')) 
					Else trim(to_char(lsa.lsavalorempenhado, '0D99'))
				End as lsavalorempenhado,
				
				trim(replace(to_char(lsa.lsaalunoequivalente, '9999999999G999'), ',', '.')) as lsaalunoequivalente,
				
				Case When ( lsa.lsavalorempenhado/lsa.lsaalunoequivalente ) > 1
					Then trim( to_char( ( lsa.lsavalorempenhado/lsa.lsaalunoequivalente ), '999G999G999G999G999D99' ) ) 
					Else trim( to_char( ( lsa.lsavalorempenhado/lsa.lsaalunoequivalente ), '0D99' ) ) 
				End as lsavaloralunoequivalente,
				
				replace( cast( (lsa.lsavalorempenhado/lsa.lsaalunoequivalente ) as text), '.', ',') as h_lsavaloralunoequivalente,
				--percentual %				
				--replace(cast(round((lsametavalorreducaoaluno * 100)/lsavaloralunoequivalente, 2) as text), '.',',') as  lsametareducaoaluno,
				
				Case When substr(cast(lsavaloralunoequivalente as text), 1, 4) <> '0.00' 
					Then replace(cast(round((lsametavalorreducaoaluno * 100)/lsavaloralunoequivalente, 2) as text), '.',',') 
					Else replace(substr(cast(lsavaloralunoequivalente as text), 1, 4), '.', ',')
				End as  lsametareducaoaluno,
				
				replace(cast(lsa.lsametareducaoaluno as text), '.', ',') as h_lsametareducaoaluno,
				
				Case When lsa.lsametavalorreducaoaluno > 1
					Then trim(to_char(lsa.lsametavalorreducaoaluno, '999G999G999G999G999D99' ) ) 
					Else trim(to_char(lsa.lsametavalorreducaoaluno, '0D99' ) )
				End as lsametavalorreducaoaluno,
				
				replace( cast( lsa.lsametavalorreducaoaluno as text), '.', ',') as h_lsametavalorreducaoaluno,
				--totais meta economia proposta
				Case When (lsaalunoequivalente * lsametavalorreducaoaluno) > 1
					Then trim(to_char(lsaalunoequivalente * lsametavalorreducaoaluno, '999G999G999G999G999D99' ) ) 
					Else trim(to_char(lsaalunoequivalente * lsametavalorreducaoaluno, '0D99' ) )
				End as valortotal,
				
				Case When (lsavaloralunoequivalente - lsametavalorreducaoaluno) > 1 
					Then trim(to_char(lsavaloralunoequivalente - lsametavalorreducaoaluno, '999G999G999G999G999D99' ) ) 
					Else trim(to_char(lsavaloralunoequivalente - lsametavalorreducaoaluno, '0D99' ) )
				End as desppactuadaequival,
				
				--Totais
				trim(to_char(t.total_valorempenhado, '999G999G999G999G999D99')) as total_valorempenhado,
				t.total_alunoequivalente,
				trim(to_char(t.total_valoralunoequivalente, '999G999G999G999G999D99')) as total_valoralunoequivalente,
				replace(cast(t.total_metareducaoaluno as text), '.', ',') as total_metareducaoaluno,
				trim(to_char(t.total_metavalorreducaoaluno, '999G999G999G999G999D99')) as total_metavalorreducaoaluno,
				trim(to_char(t.total_valortotal, '999G999G999G999G999D99')) as total_valortotal,
				trim(to_char(t.total_desppactuadaequival, '999G999G999G999G999D99')) as total_desppactuadaequival 
		From elabrev.despesasustentavel ds
	
		Join (
			Select	unicod, 
					lsastatus,
					sum(lsavalorempenhado) as total_valorempenhado,
					sum(lsaalunoequivalente) as total_alunoequivalente,
					sum(round( (lsavalorempenhado/lsaalunoequivalente), 2) ) as total_valoralunoequivalente,
					( (  sum(lsaalunoequivalente * lsametavalorreducaoaluno) / sum(lsavalorempenhado) ) *100 ) as total_metareducaoaluno,
					sum(lsametavalorreducaoaluno) as total_metavalorreducaoaluno,
					sum(lsaalunoequivalente * lsametavalorreducaoaluno) as total_valortotal,
					sum(lsavaloralunoequivalente - lsametavalorreducaoaluno) as total_desppactuadaequival
			From elabrev.lancamentosustentavelaluno
			Where lsastatus = 'A'
			Group by unicod, lsastatus
		) t on  t.unicod = '".$unicod."'
	
		Left Join (
			Select	dpsid,
					unicod,
					entid,
					lsastatus,
					lsavalorempenhado,
					lsaalunoequivalente,
					lsavaloralunoequivalente,
					lsametareducaoaluno,
					lsametavalorreducaoaluno
			From elabrev.lancamentosustentavelaluno
			Where lsastatus = 'A'
		) lsa on lsa.dpsid = ds.dpsid and lsa.unicod = '".$unicod."'
	
		Where ds.dpsstatus = 'A' and (t.lsastatus = 'A' and lsa.lsastatus = 'A')
		Order by dpsid
		";
        
        // Carregando as despesas.
        $despesas = $db->carregar($sql);
        
        $percentual = str_replace( ',', '.', str_replace('.', '', $despesas[0]['total_metareducaoaluno']));
        
        //Inserindo percentual da uo.
        $resultInserirMentaFinanceira = $clsSoapSispesPlanejamento->inserirMetaFinanceira($unicod, $percentual);
        
        if (!isset($resultInserirMentaFinanceira->return->mensagem) || $resultInserirMentaFinanceira->return->mensagem != 'INCLUSAO EFETUADA COM SUCESSO') {
            echo '<script lang="javascript">alert("O processo de enviar as metas parou, pois ouve um erro no web service ao tentar enviar o percentual da UO!");</script>';
        } else {
        
            // Montando array com os dados para inserir no WebService SisPES.
            $arrInserirDespesaEntidade = array();
            $arrInserirDespesaEntidade['anoExercicio'] = date('Y'); // int
            $arrInserirDespesaEntidade['codigoEntidade'] = $coEntidade; // int
            $arrInserirDespesaEntidade['codigoUo'] = $unicod; // string

            foreach ($despesas as $key => $meta) {
                $arrInserirDespesaEntidade['despesas'][$key]['codigoDespesa'] = $meta['dpsidsispes']; // despesaEntidadeDTO
                $arrInserirDespesaEntidade['despesas'][$key]['valor'] = str_replace(
                        ',', '.', str_replace('.', '', $meta['valortotal'])); // despesaEntidadeDTO
            }

            $result = $clsSoapSispesPlanejamento->inserirMetaEntidade($arrInserirDespesaEntidade);

            // Se o WebService não tiver retornado a msg como esperado, para o loop, muda o status do $booOk para nao salvar a data e exibe uma msg informando o usuario.
            if (!isset($result->return->mensagem) || $result->return->mensagem != 'INCLUSAO EFETUADA COM SUCESSO') {
                echo '<script lang="javascript">alert("O processo de enviar as metas parou pois ouve um erro no web service ao tentar enviar a despes!");</script>';
            } else {
                $sql = "UPDATE elabrev.unidadeordenadora 
                        SET codentidadesispes = {$coEntidade}, dataenviosispes = now()
                        WHERE unicod = '{$unicod}'";
                        $db->executar($sql);
                        $db->commit();
                echo '<script lang="javascript">alert("Metas enviadas ao SisPES com sucesso!");</script>';
            }
        }
        
        // Loop com as despesas para enviar uma a uma.
//        foreach($despesas as $meta){
//            $arrInserirDespesaEntidade['despesas']['codigoDespesa'] = $meta['dpsidsispes']; // despesaEntidadeDTO
////            $arrInserirDespesaEntidade['despesas']['valor'] = str_replace('.', '', str_replace(',', '.', $meta['valortotal'])); // despesaEntidadeDTO
//            $arrValor = explode(',' , $meta['valortotal']); // despesaEntidadeDTO
//            $arrInserirDespesaEntidade['despesas']['valor'] = str_replace('.', '', $arrValor[0]); // despesaEntidadeDTO
//
//            // Enviando uma despesa para o WebService SisPES.
//            $result = $clsSoapSispesPlanejamento->inserirMetaEntidade($arrInserirDespesaEntidade);
//            $booOk = true;
//            
//            // Se o WebService não tiver retornado a msg como esperado, para o loop, muda o status do $booOk para nao salvar a data e exibe uma msg informando o usuario.
//            if(!isset($result->return->mensagem) || $result->return->mensagem != 'INCLUSAO EFETUADA COM SUCESSO'){
//
//                $booOk = false;
//                echo '<script lang="javascript">alert("O processo de enviar as metas parou pois ouve um erro no web service com a despesa '.$meta['dpsidsispes'].' !");</script>';
//                break;
//            }
//        }
//
//        // Se todas as despesas tiverem sido salvas com sucesso ele salva na tabela o codigo da entidade do SisPES que representa a UO no simec e exibe msg de sucesso para o usuario.
//        if($booOk){
//            $sql = "UPDATE elabrev.unidadeordenadora 
//                    SET codentidadesispes = {$coEntidade}, dataenviosispes = now()
//                    WHERE unicod = '{$unicod}'";
//                    $db->executar($sql);
//                    $db->commit();
//            echo '<script lang="javascript">alert("Metas enviadas ao SisPES com sucesso!");</script>';
//        }
    
    } else {
        
        // Se nao encontrou uma entidade na UO dentro do WebService SisPES ele exibe a msg informando o usuario.
        echo '<script lang="javascript">alert("Não pode enviar as metas anuais pois não encontrou nenhuma entidade cadastrada com esta UO!");</script>';
    }
}
?>


<style>
    .div_info{
        border-style: 1px;
        border-color: black;
        font-family: arial, serif, sans-serif;
        font-size: 18px;
        background-color: #D3D3D3;
        visibility: hidden;
        position: absolute;
        width: 28%;
        height:auto;
        z-index: 100;
    }
</style>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">

    $(document).ready(function() {

        $('input:text[name="lsametavalorreducaoaluno[]"]').attr('disabled', true);

        $('[name="hab_campo"]').click(function() {
            var param = $('input:radio[name=hab_campo]:checked').val();

            if (param == 'P') {
                $('input:text[name="lsametareducaoaluno[]"]').attr('disabled', false);
                $('input:text[name="lsametavalorreducaoaluno[]"]').attr('disabled', true);
            }

            if (param == 'V') {
                $('input:text[name="lsametareducaoaluno[]"]').attr('disabled', true);
                $('input:text[name="lsametavalorreducaoaluno[]"]').attr('disabled', false);
            }
        });


        //% Redução por aluno equivalente.
        $('[name="lsametareducaoaluno[]"]').keyup(function() {
            var linha = $(this).attr('id');
            linha = linha.split('_');

            //Despesa por Aluno Equivalente.
            var h_lsavaloralunoequivalente = replaceAll(replaceAll($('#h_lsavaloralunoequivalente_' + linha[1]).val(), ".", ""), ",", ".");
            //% Redução por aluno equivalente.
            var lsametareducaoaluno = replaceAll($(this).val(), ",", ".");

            //Valor por aluno equivalente - Visualização em tela.
            var lsametavalorreducaoaluno = ((h_lsavaloralunoequivalente * lsametareducaoaluno) / 100);
            $('#lsametavalorreducaoaluno_' + linha[1]).val(mascaraglobal("[.###],##", lsametavalorreducaoaluno.toFixed(2)));
            //Valor por aluno equeivalente - Hidden.
            $('#h_lsametavalorreducaoaluno_' + linha[1]).val(replaceAll(lsametavalorreducaoaluno, ".", ","));


            /*RETORNAR O VALOR PARA O CAMPO (% Redução por aluno equivalente) HIDDEN COM O CALCULO FEITO MATENDO OS VALORES NA SUA INTEGRIDADE*/
            var h_lsametavalorreducaoaluno = replaceAll($('#h_lsametavalorreducaoaluno_' + linha[1]).val(), ",", ".");
            var h_lsametareducaoaluno = (h_lsametavalorreducaoaluno / h_lsavaloralunoequivalente) * 100;
            $('#h_lsametareducaoaluno_' + linha[1]).val(replaceAll(h_lsametareducaoaluno.toFixed(14), ".", ","));

            /*CASO O CAMPO "DESPESA POR ALUNO" TIVER VALOR 0,00 É COLOCADO UM VALOR 0,00 NO CAMPO "% REDUÇÃO POR ALUNO" - HIDDEN*/
            if (parseFloat(h_lsavaloralunoequivalente).toFixed(2) == 0.00) {
                $('#h_lsametareducaoaluno_' + linha[1]).val('0,00');
            }

            valorTotal(linha[1]);

        });

        //Valor por aluno equivalente 
        $('[name="lsametavalorreducaoaluno[]"]').keyup(function() {
            var linha = $(this).attr('id');
            linha = linha.split('_');

            var h_lsavaloralunoequivalente = replaceAll($('#h_lsavaloralunoequivalente_' + linha[1]).val(), ",", ".");//Despesa por Aluno Equivalente
            var h_lsametavalorreducaoaluno = replaceAll($('#h_lsametavalorreducaoaluno_' + linha[1]).val(), ",", "."); //Valor por aluno equivalente hidden.
            var lsametavalorreducaoaluno = replaceAll($(this).val(), ",", "."); //Valor por aluno equivalente.

            var lsametareducaoaluno = (lsametavalorreducaoaluno / h_lsavaloralunoequivalente) * 100;
            $('#lsametareducaoaluno_' + linha[1]).val(mascaraglobal("[#],##", lsametareducaoaluno.toFixed(2)));
            //% Redução por aluno equivalente - Hidden.
            $('#h_lsametareducaoaluno_' + linha[1]).val(replaceAll(lsametareducaoaluno, ".", ","));

            /*CASO O CAMPO "DESPESA POR ALUNO" TIVER VALOR 0,00 É COLOCADO UM VALOR 0,00 NO CAMPO "% REDUÇÃO POR ALUNO"*/
            if (parseFloat(h_lsavaloralunoequivalente).toFixed(2) == 0.00) {
                $('#h_lsametareducaoaluno_' + linha[1]).val('0,00');
                $('#lsametareducaoaluno_' + linha[1]).val('0,00');
            }

            /*RETORNAR O VALOR PARA O CAMPO (Valor por aluno equivalente) HIDDEN COM O CALCULO FEITO MATENDO OS VALORES NA SUA INTEGRIDADE*/
            var h_lsametareducaoaluno = replaceAll($('#h_lsametareducaoaluno_' + linha[1]).val(), ",", ".");
            var h_lsametavalorreducaoaluno = ((h_lsavaloralunoequivalente * h_lsametareducaoaluno) / 100);
            $('#h_lsametavalorreducaoaluno_' + linha[1]).val(replaceAll(h_lsametavalorreducaoaluno, ".", ","));

            valorTotal(linha[1]);

        });

        function valorTotal(linha) {
            var linha = linha;

            //Aluno Equivalente.
            var lsaalunoequivalente = replaceAll($('#lsaalunoequivalente_' + linha).val(), ".", "");

            //Valor por aluno equivalente.
            var lsametavalorreducaoaluno = replaceAll($('#lsametavalorreducaoaluno_' + linha).val(), ",", ".");
            //Valor por aluno equivalente - Hidden.
            var h_lsametavalorreducaoaluno = replaceAll($('#h_lsametavalorreducaoaluno_' + linha).val(), ",", ".");

            if (h_lsametavalorreducaoaluno != '' || h_lsametavalorreducaoaluno > 0) {
                lsametavalorreducaoaluno = h_lsametavalorreducaoaluno;
            } else {
                lsametavalorreducaoaluno = lsametavalorreducaoaluno;
            }

            var valortotal = (lsametavalorreducaoaluno * lsaalunoequivalente);
            $('#valortotal_' + linha).val(mascaraglobal("[.###],##", valortotal.toFixed(2)));
            $('#h_valortotal_' + linha).val(replaceAll(valortotal, ".", ","));

            despesaPactuada(linha);
        }

        function despesaPactuada(linha) {
            var linha = linha;

            var h_lsavaloralunoequivalente = replaceAll($('#h_lsavaloralunoequivalente_' + linha).val(), ",", ".");

            //Valor por aluno equivalente.
            var lsametavalorreducaoaluno = replaceAll($('#lsametavalorreducaoaluno_' + linha).val(), ",", ".");
            //Valor por aluno equivalente - Hidden.
            var h_lsametavalorreducaoaluno = replaceAll($('#h_lsametavalorreducaoaluno_' + linha).val(), ",", ".");


            if (h_lsametavalorreducaoaluno != '') {
                lsametavalorreducaoaluno = h_lsametavalorreducaoaluno;
            } else {
                lsametavalorreducaoaluno = lsametavalorreducaoaluno;
            }

            var desppactuadaequival = h_lsavaloralunoequivalente - lsametavalorreducaoaluno;
            $('#desppactuadaequival_' + linha).val(mascaraglobal("[.###],##", desppactuadaequival.toFixed(2)));

            totalizador();
        }

        function totalizador() {

            var total_metavalorreducaoaluno = 0;
            $.each($('[name="h_lsametavalorreducaoaluno[]"]'), function(i, v) {
                if (($(v).val() != '')) {
                    var lsametavalorreducaoaluno = replaceAll(replaceAll($(v).val(), ".", ""), ",", ".");
                    total_metavalorreducaoaluno = parseFloat(total_metavalorreducaoaluno) + parseFloat(lsametavalorreducaoaluno);
                }
            });
            $('#total_metavalorreducaoaluno').val(mascaraglobal("[.###],##", total_metavalorreducaoaluno.toFixed(2)));

            var total_valortotal = 0;
            $.each($('[name="h_valortotal[]"]'), function(i, v) {
                if (($(v).val() != '')) {
                    var valortotal = replaceAll(replaceAll($(v).val(), ".", ""), ",", ".");
                    total_valortotal = parseFloat(total_valortotal) + parseFloat(valortotal);
                }
                valortotal = 0;
            });
            $('#total_valortotal').val(mascaraglobal("[.###],##", total_valortotal.toFixed(2)));
            $('#h_total_valortotal').val(total_valortotal);

            var total_desppactuadaequival = 0;
            $.each($('[name="desppactuadaequival[]"]'), function(i, v) {
                if (($(v).val() != '')) {
                    var desppactuadaequival = replaceAll(replaceAll($(v).val(), ".", ""), ",", ".");
                    total_desppactuadaequival = parseFloat(total_desppactuadaequival) + parseFloat(desppactuadaequival);
                }
            });
            $('#total_desppactuadaequival').val(mascaraglobal("[.###],##", total_desppactuadaequival.toFixed(2)));

            totalPercentual();
        }

        function totalPercentual() {

            //Percentual do valor pactuado - total_valorempenhado.
            var total_valorempenhado = replaceAll(replaceAll($('[name="total_valorempenhado"]').val(), ".", ""), ",", ".");
            var h_total_valortotal = replaceAll($('[name="h_total_valortotal"]').val(), ",", ".");

            var total_metareducaoaluno = ((parseFloat(h_total_valortotal) / parseFloat(total_valorempenhado)) * 100);
            $('#total_metareducaoaluno').val(replaceAll(total_metareducaoaluno, ".", ","));

            if (total_metareducaoaluno >= 10.00) {
                $('#total_valortotal, #total_metavalorreducaoaluno, #total_metareducaoaluno').css("color", "green");
            } else {
                $('#total_metareducaoaluno, #total_metavalorreducaoaluno, #total_valortotal').css("color", "red");
            }

        }

        $('[name="lsametavalorreducaoaluno[]"]').blur(function() {
            var linha = $(this).attr('id');
            linha = linha.split('_');

            var lsametavalorreducaoaluno = replaceAll($(this).val(), ",", "."); //Valor por aluno equivalente.
            var lsavaloralunoequivalente = replaceAll($('#lsavaloralunoequivalente_' + linha[1]).val(), ",", ".");//Despesa por Aluno Equivalente

            if (lsametavalorreducaoaluno > lsavaloralunoequivalente) {
                alert('O valor que esta sendo definido é maior que 100% do valor da Despesa por aluno.');
            }
        });

        $('[name="lsametareducaoaluno[]"]').blur(function() {
            var linha = $(this).attr('id');
            linha = linha.split('_');

            var lsametareducaoaluno = replaceAll($(this).val(), ",", "."); //Valor por aluno equivalente.

            if (lsametareducaoaluno > 100) {
                alert('O valor que esta sendo definido é maior que 100% do valor da Despesa por aluno.');
            }
        });

        $('.salvar').click(function() {
            $('#requisicao').val('salvarPactoAluno');
            var valida = validarCampos();
            if (valida) {
                $('#formulario').submit();
            }
        });
    });

    function validarCampos() {
        var erro = 0;

        var lsametareducaoaluno_1 = replaceAll(replaceAll($('#lsametareducaoaluno_1').val(), ".", ""), ",", ".");
        var lsametareducaoaluno_4 = replaceAll(replaceAll($('#lsametareducaoaluno_4').val(), ".", ""), ",", ".");
        var lsametareducaoaluno_9 = replaceAll(replaceAll($('#lsametareducaoaluno_9').val(), ".", ""), ",", ".");

        if (parseFloat(lsametareducaoaluno_1) <= 0.00 || parseFloat(lsametareducaoaluno_4) <= 0.00 || parseFloat(lsametareducaoaluno_9) <= 0.00) {
            erro = 1;
        }

        $.each($('[name="lsavaloralunoequivalente[]"]'), function(i, v) {
            if (($(v).val() == '')) {
                erro = 2;
            }
        });

        $.each($('[name="lsametareducaoaluno[]"]'), function(i, v) {
            if (($(v).val() == '')) {
                erro = 3;
            }
        });

        $.each($('[name="lsametavalorreducaoaluno[]"]'), function(i, v) {
            if (($(v).val() == '')) {
                erro = 4;
            }
        });

        $.each($('[name="lsametareducaoaluno[]"]'), function(i, v) {
            var valor = replaceAll($(v).val(), ",", ".");
            if ((valor > 100)) {
                erro = 6;
            }
        });

        //Verifica se o valor de 10% foi alcançado.			
        var total_metareducaoaluno = replaceAll($('#total_metareducaoaluno').val(), ",", ".");

        if (total_metareducaoaluno < 10.00) {
            erro = 5;
        }

        if (erro == 1) {
            alert('Os campos referentes a "Água e Esgoto, Energia Elétrica e Material de consumo", devem ter uma redução maior que zero %!');
            validacao = false;
        } else if (erro == 2 || erro == 3 || erro == 4) {
            alert('A campos em branco no formulário, todos os campos devem ser preenchidos!');
            validacao = false;
        } else if (erro == 5) {
            alert('Meta de Economia Proposta de 10% não foi atingido!');
            validacao = false;
        } else if (erro == 6) {
            alert('Existem valores maiores que 100% do valor da Despesa Alunos.');
            validacao = false;
        } else {
            validacao = true;
        }
        return validacao;
    }

    //Variaveis Globais - Pega a posição do mouse.
    var Tleft = 0;
    var Ttopo = 0;

    $("html").mousemove(function(mouse) {
        Tleft = mouse.pageX + 10;
        Ttopo = mouse.pageY + 20;
    });

    function titleNaturezaInfoVisibilyt(dpsid) {
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicao=titleNaturezaDespesa&dpsid=" + dpsid,
            async: false,
            success: function(msg) {
                $(".div_info").css({visibility: "visible"});
                $(".div_info").html(msg);
                $('.div_info').css("top", Ttopo);
                $('.div_info').css("left", Tleft);
            }
        });
    }

    function titleNaturezaInfoHidden() {
        $(".div_info").css({visibility: "hidden"});
    }
</script>

<?php
$dados = buscaResObservacao($unicod);

$uo = $db->pegaLinha("SELECT codentidadesispes, to_char(dataenviosispes, 'DD/MM/YYYY') as dataenviosispes 
                      FROM elabrev.unidadeordenadora codentidadesispes 
                      WHERE unicod = '{$unicod}'");
?>

<form name="formulario" id="formulario" method="POST" action="">
    <input type="hidden" id="requisicao" name="requisicao" value="" />

    <div class="div_info"> </div>

    <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <th style="background-color: #A9A9A9; font-size: 12px; text-align: left; width: 40%;">UO: <?php echo $unicod . ' - ' . $unidade_decricao; ?> </th>
            <th style="background-color: #A9A9A9; font-size: 14px; "> Projeto Esplanada Sustentável</th>
        </tr>
        <tr>
            <th colspan="2" style="background-color: #A9A9A9; font-size: 12px; text-align: left;">Ano exercício atual: <?= date('Y'); ?></th>
        </tr>
        <?php if($uo['codentidadesispes'] && $uo['dataenviosispes']): ?>
        <tr>
            <th style=" font-size: 12px; text-align: left; width: 35%; ">Código da entidade no SisPES: <?php echo $uo['codentidadesispes'] ?> </th>
            <th style=" font-size: 14px;">Data do último envio ao SisPES: <?php echo $uo['dataenviosispes'] ?></th>
        </tr>
        <?php endif ?>
        <tr>
            <th colspan="2" style="font-size: 12px; text-align: left;">Meta de despesa 2013 Pactuada por aluno equivalente</th>
        </tr>
    </table>

    <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <th rowspan="3" width="22%">Despesa</th>
            <th rowspan="3" width="12%">Despesas Liquidadas 2012 <br> (A)</th>
            <th rowspan="3" width="8%">Aluno Equivalente <br> (B)</th>
            <th rowspan="3" width="12%">Despesa por Aluno Equivalente <br>(C) = (A / B)</th>
            <th colspan="3" width="36%">Meta de Economia Proposta</td>
            <th rowspan="3" width="21%">Despesa 2013 pactuada por aluno equivalente <br> (G) = (C - E)</th>
        </tr>
        <tr>
            <th width="12%">% Redução por aluno equivalente <br> (D) = (E / C)%</th>
            <th width="12%">Valor por aluno equivalente <br> (E) = (C x D)</th>
            <th width="12%" rowspan="2">Valor Total <br> (F) = (B x E)</th>
        </tr>
        <tr>
            <th><input id="habil_perce" name="hab_campo" type="radio" value="P" checked="checked"></th>
            <th><input id="habil_valor" name="hab_campo" type="radio" value="V"></th>
        </tr>
        <?php
        listaItensDespesasaAlunos($unicod);

        require_once APPRAIZ . "includes/workflow.php";

        $docid = buscarDocidEsplanada($unicod);
        //ver($docid);
        
        $estado = pegaEstadoAtualEsplanada($docid);
        ?>
    </table>

    <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <th style="text-align: left;" colspan="2">Observações da UO:</th>
        </tr>
        <tr>
            <td>
                <?php echo campo_textarea('lsaobservacao', 'N', 'S', 'Observação ', 165, 2, 2000, '', '', '', '', 'id="lsaobservacao', $dados['lsaobservacao'], ''); ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" style="text-align: center;">
<?php
$perfil = array();
$perfil = pegaPerfilGeral($_SESSION['usucpf'], $_SESSION['sisid']);

if (( $estado == '' || $estado == WF_ES_EM_CADASTRAMENTO_ALUNOS ) &&
        (
        in_array(UO_COORDENADOR_EQUIPE_TECNICA, $perfil) ||
        in_array(UO_EQUIPE_TECNICA, $perfil) ||
        in_array(PERFIL_PROREITOR_ADM, $perfil) ||
        in_array(PERFIL_REITOR, $perfil)
        )
) {
    ?>
                    <input type="button" id="salvar" class="salvar" name="salvar" value="Salvar">
                <?php } elseif ($estado == WF_ES_EM_VALIDACAO_PROREITOR_ALUNOS && ( in_array(PERFIL_PROREITOR_ADM, $perfil) || in_array(PERFIL_REITOR, $perfil) )) { ?>
                    <input type="button" id="salvar" class="salvar" name="salvar" value="Salvar">
                <?php } elseif (in_array(PERFIL_SUPER_USUARIO, $perfil)) { ?>
                    <input type="button" id="salvar" class="salvar" name="salvar" value="Salvar">
                <?php } else { ?>
                    <input type="button" value="Salvar" disabled="disabled">
                <?php } ?>
            </td>
        </tr>
    </table>
</form>

<?php if (in_array(PERFIL_SUPER_USUARIO, $perfil)) : ?>
    <form name="form_enviar_dados" method="post">
        <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
            <input type="hidden" name="boolean_enviar_dados" value="1">
            <tr>
                <td><input type="submit" id="enviar_dados" class="salvar" name="enviar_dados" value="Enviar dados"></td>
            </tr>
        </table>
    </form>
<?php endif; ?>

<table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <th colspan="2" style="text-align: left;">Notas:</th>
    </tr>
    <tr>
        <td width="50%" style="border:thin; border-style:solid; border-color:#BEBEBE;">
            - Despesas Liquidadas na LOA 2012 (Contas Contábeis = +292130301-292130203+292130203 +292130201+292130202), Fonte: SIAFI Gerencial.<br>
<?php echo notaRodaPe($unicod); ?>
        </td>
        <td width="50%" align="justify" style="border:thin; border-style:solid; border-color:#BEBEBE;">
            - Para fins de padronização, os montantes refereUO: ntes às operações de cálculos efetuados acima estão expressos em duas casas decimas, com aproximação a partir de valores absolutos.
        </td>
    </tr>
</table>

<td align="center">
<?php
$dados_wf = array("unicod" => $unicod, "esplaAluno" => 'S');

if ($docid != '') {
    wf_desenhaBarraNavegacao($docid, $dados_wf);
}

//$tSql = "select * from elabrev.lancamentosustentavelaluno order by lsaid asc";
//$rSet = $db->carregar($tSql);
//$i = 0;
//$pk = 1858;
//if ($rSet) {
//    foreach ($rSet as $row) {
//        
//        $lsavalorempenhado = $row['lsavalorempenhado'] ? $row['lsavalorempenhado'] : 'null';
//        $lsaalunoequivalente = $row['lsaalunoequivalente'] ? $row['lsaalunoequivalente'] : 'null';
//        $lsavaloralunoequivalente = $row['lsavaloralunoequivalente'] ? $row['lsavaloralunoequivalente'] : 'null';
//        $lsametareducaoaluno = $row['lsametareducaoaluno'] ? $row['lsametareducaoaluno'] : 'null';
//        $lsametavalorreducaoaluno = $row['lsametavalorreducaoaluno'] ? $row['lsametavalorreducaoaluno'] : 'null';
//        
//        $iSql = "INSERT INTO elabrev.lancamentosustentavelaluno
//                (lsaid,
//                dpsid,
//                entid, 
//                unicod, 
//                lsavalorempenhado, 
//                lsaalunoequivalente, 
//                lsavaloralunoequivalente, 
//                lsametareducaoaluno, 
//                lsametavalorreducaoaluno, 
//                lsastatus, 
//                lsaobservacao, 
//                anoexercicio) 
//                VALUES 
//                ($pk,
//                {$row['dpsid']},
//                null,
//                '{$row['unicod']}',
//                $lsavalorempenhado,
//                $lsaalunoequivalente,
//                $lsavaloralunoequivalente,
//                $lsametareducaoaluno,
//                $lsametavalorreducaoaluno,
//                '{$row['lsastatus']}',
//                'teste',
//                '2014')";
//        
//        $db->executar($iSql);
//        $db->commit();
//        $i++;
//        $pk++;
//    }
//    echo 'sucesso = '.$i;
//}

?>
</td>	
