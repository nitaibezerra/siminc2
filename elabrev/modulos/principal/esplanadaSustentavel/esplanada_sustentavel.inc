<?php
require_once '_funcoes_esplanada_sustentavel.php';

if ($_REQUEST['requisicao']) {
    $_REQUEST['requisicao']( $_REQUEST );
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
    
    // Pegando a entidade no WebService SisPES de acordo com a UO.
    foreach($result->return->uos as $uo){
        if(is_array($uo->entidades)){
            $coEntidade = $uo->entidades[0]->codigo;
        } else {
            if(isset($uo->entidades->codigo) && $uo->codigo == $unicod)
            $coEntidade = $uo->entidades->codigo;
        }
    }
    
    // Se tiver entidade ele comeca o processo de salvar as despesas.
    if(isset($coEntidade)){
        $sql = "
			Select	ds.dpsid,
					ds.dpsdescricao,
                    ds.dpsidsispes,
					Case When ls.lcsvalorempenhado > 1
						Then trim(to_char(ls.lcsvalorempenhado, '999G999G999G999G999D99')) 
						Else trim(to_char(ls.lcsvalorempenhado, '0D99'))
					End as lcsvalorempenhado,
					replace(cast(ls.lcsvalormeta as text), '.', ',') as lcsvalormeta,
					Case When ls.lcsvalordeducao > 1
						Then trim(to_char(ls.lcsvalordeducao, '999G999G999G999G999D99' ) ) 
						Else trim(to_char(ls.lcsvalordeducao, '0D99' ) )
					End as lcsvalordeducao,
					--Totais
					trim(to_char(t.totalvalorempenhado, '999G999G999G999G999D99')) as totalvalorempenhado,
					--trim(to_char(t.totalvalormeta, '99D99')) as totalvalormeta,
					replace(cast(t.totalvalormeta as text), '.', ',') as totalvalormeta,
					trim(to_char(t.totalvalordeducao, '999G999G999G999G999D99')) as totalvalordeducao
			From elabrev.despesasustentavel ds
				
			Join (
				Select	unicod,
						sum(lcsvalorempenhado) as totalvalorempenhado,
						( (  sum(lcsvalordeducao) / sum(lcsvalorempenhado) ) *100 ) as totalvalormeta,
						sum(lcsvalordeducao) as totalvalordeducao
				From elabrev.lancamentosustentavel
				Where lcsstatus = 'A'
				Group by unicod
			) t on  t.unicod = '".$unicod."'
				
			Left Join (
				Select	dpsid,
						unicod,
						entid,
						lcsvalorempenhado,
						lcsvalormeta,
						lcsvalordeducao
				From elabrev.lancamentosustentavel
				Where lcsstatus = 'A'
			) ls on ls.dpsid = ds.dpsid and ls.unicod = '".$unicod."'
				
			Where ds.dpsstatus = 'A'
			Order by dpsid
		";
        // Carregando as despesas.
        $despesas = $db->carregar($sql);
        
        // Montando array com os dados para inserir no WebService SisPES.
        $arrInserirDespesaEntidade = array();
        $arrInserirDespesaEntidade['anoExercicio'] = date('Y'); // int
        $arrInserirDespesaEntidade['codigoEntidade'] = $coEntidade; // int
        $arrInserirDespesaEntidade['codigoUo'] = $unicod; // string

        foreach($despesas as $key => $meta){
            $arrInserirDespesaEntidade['despesas'][$key]['codigoDespesa'] = $meta['dpsidsispes']; // despesaEntidadeDTO
            $arrInserirDespesaEntidade['despesas'][$key]['valor'] = str_replace( ',', '.', str_replace('.', '', $meta['lcsvalordeducao'])); // despesaEntidadeDTO
        }
        
        $percentual = str_replace( ',', '.', str_replace('.', '', $despesas[0]['totalvalorempenhado']));
        
        //Inserindo percentual da uo.
        $resultInserirMentaFinanceira = $clsSoapSispesPlanejamento->inserirMetaFinanceira($unicod, $percentual);
        
        if(!isset($resultInserirMentaFinanceira->return->mensagem) || $resultInserirMentaFinanceira->return->mensagem != 'INCLUSAO EFETUADA COM SUCESSO'){
            echo '<script lang="javascript">alert("O processo de enviar as metas parou, pois ouve um erro no web service ao tentar enviar o percentual da UO!");</script>';
        } else {
            
            // Inserindo metas anuais da entidade.
            $result = $clsSoapSispesPlanejamento->inserirMetaEntidade($arrInserirDespesaEntidade);
        
            // Se o WebService não tiver retornado a msg como esperado, para o loop, muda o status do $booOk para nao salvar a data e exibe uma msg informando o usuario.
            if(!isset($result->return->mensagem) || $result->return->mensagem != 'INCLUSAO EFETUADA COM SUCESSO'){
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
////            $arrInserirDespesaEntidade['despesas']['valor'] = str_replace('.', '', str_replace(',', '.', $meta['lcsvalordeducao'])); // despesaEntidadeDTO
////            $arrInserirDespesaEntidade['despesas']['valor'] = str_replace('.', '', $meta['lcsvalordeducao']); // despesaEntidadeDTO
//            $arrValor = explode(',' , $meta['lcsvalordeducao']); // despesaEntidadeDTO
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

    $( document ).ready( function() {
        $( '[name="lcsvalormeta[]"]' ).keyup( function() {
            var linha = $( this ).attr( 'id' );
            linha = linha.split( '_' );

            var lcsvalormeta = replaceAll( $( this ).val(), ",", "." );
            var lcsvalorempenhado = replaceAll( replaceAll( $( '#lcsvalorempenhado_' + linha[1] ).val(), ".", "" ), ",", "." );
            var lcsvalordeducao = parseFloat( (lcsvalorempenhado * lcsvalormeta) / 100 );

            $( '#lcsvalordeducao_' + linha[1] ).val( mascaraglobal( "[.###],##", lcsvalordeducao.toFixed( 2 ) ) );

            var totalValordeducao = 0
            $.each( $( '[name="lcsvalordeducao[]"]' ), function( i, v ) {
                if ( ($( v ).val() != '') ) {
                    var valordeducao = replaceAll( replaceAll( $( v ).val(), ".", "" ), ",", "." );
                    totalValordeducao = parseFloat( totalValordeducao ) + parseFloat( valordeducao );
                }
            } );

            //Despesas Empenhada 2012 
            var totalValorempenhado = replaceAll( replaceAll( $( '[name="totalvalorempenhado"]' ).val(), ".", "" ), ",", "." );
            //Meta de Economia Proposta - % 
            var totalValormeta = (parseFloat( totalValordeducao ) / totalValorempenhado) * 100;

            if ( (totalValormeta >= 10.00) ) {
                $( '#totalvalordeducao' ).val( mascaraglobal( "[.###],##", totalValordeducao.toFixed( 2 ) ) ).css( "color", "green" );
                $( '#totalvalormeta' ).val( totalValormeta ).css( "color", "green" );
            } else {
                $( '#totalvalordeducao' ).val( mascaraglobal( "[.###],##", totalValordeducao.toFixed( 2 ) ) ).css( "color", "red" );
                $( '#totalvalormeta' ).val( totalValormeta ).css( "color", "red" );
            }
        } );

        $( '[name="lcsvalormeta[]"]' ).blur( function() {
            var linha = $( this ).attr( 'id' );
            linha = linha.split( '_' );

            var lcsvalormeta = replaceAll( $( this ).val(), ",", "." ); //Valor por aluno equivalente.

            if ( lcsvalormeta > 100 ) {
                alert( 'O valor que esta sendo definido é maior que 100% do valor da Despesa.' );
            }
        } );

        $( '.salvar' ).click( function() {
            $( '#requisicao' ).val( 'salvarPacto' );
            var valida = validarCampos();
            if ( valida ) {
                $( '#formulario' ).submit();
            }
        } );
    } );

    function validarCampos() {
        var erro = 0;

        var lcsvalormeta_1 = replaceAll( replaceAll( $( '#lcsvalormeta_1' ).val(), ".", "" ), ",", "." );
        var lcsvalormeta_4 = replaceAll( replaceAll( $( '#lcsvalormeta_4' ).val(), ".", "" ), ",", "." );
        var lcsvalormeta_9 = replaceAll( replaceAll( $( '#lcsvalormeta_9' ).val(), ".", "" ), ",", "." );

        var totalValorempenhado = replaceAll( replaceAll( $( '[name="totalvalorempenhado"]' ).val(), ".", "" ), ",", "." );
        var totalValormeta = replaceAll( replaceAll( $( '#totalvalormeta' ).val(), ".", "" ), ",", "." );
        var totalvalordeducao = replaceAll( replaceAll( $( '[name="totalvalordeducao"]' ).val(), ".", "" ), ",", "." );

        //Meta de Economia Proposta - % 
        var totalvalormeta = replaceAll( $( '#totalvalormeta' ).val(), ",", "." );

        $.each( $( '[name="lcsvalordeducao[]"]' ), function( i, v ) {
            if ( ($( v ).val() == '') ) {
                erro = 1;
            }
        } );

        if ( (totalValormeta < 10.00) ) {
            erro = 2;
        }

        if ( parseFloat( lcsvalormeta_1 ) <= 0.00 || parseFloat( lcsvalormeta_4 ) <= 0.00 || parseFloat( lcsvalormeta_9 ) <= 0.00 ) {
            erro = 3;
        }

        $.each( $( '[name="lcsvalormeta[]"]' ), function( i, v ) {
            var valor = replaceAll( $( v ).val(), ",", "." );
            if ( (valor > 100) ) {
                erro = 4;
            }
        } );

        if ( erro == 1 ) {
            alert( 'A campos em branco no formulário, todos os campos devem ser preenchidos!' );
            validacao = false;
        } else if ( erro == 2 ) {
            alert( 'Meta de Economia Proposta de 10% não foi atingido!' );
            validacao = false;
        } else if ( erro == 3 ) {
            alert( 'Os campos referentes a "Água e Esgoto, Energia Elétrica e Material de consumo", devem ter uma redução maior que zero %!' );
            validacao = false;
        } else if ( erro == 4 ) {
            alert( 'Existem valores maiores que 100% do valor da Despesa.' );
            validacao = false;
        } else {
            validacao = true;
        }
        return validacao;
    }

    //Variaveis Globais - Pega a posição do mouse.
    var Tleft = 0;
    var Ttopo = 0;

    $( "html" ).mousemove( function( mouse ) {
        Tleft = mouse.pageX + 10;
        Ttopo = mouse.pageY + 20;
    } );

    function titleNaturezaInfoVisibilyt( dpsid ) {
        $.ajax( {
            type: "POST",
            url: window.location,
            data: "requisicao=titleNaturezaDespesa&dpsid=" + dpsid,
            async: false,
            success: function( msg ) {
                $( ".div_info" ).css( { visibility: "visible" } );
                $( ".div_info" ).html( msg );
                $( '.div_info' ).css( "top", Ttopo );
                $( '.div_info' ).css( "left", Tleft );
            }
        } );
    }

    function titleNaturezaInfoHidden() {
        $( ".div_info" ).css( { visibility: "hidden" } );
    }
</script>

<?php
$dados = valorPercentualTitulo( $unicod );

$uo = $db->pegaLinha("SELECT codentidadesispes, to_char(dataenviosispes, 'DD/MM/YYYY') as dataenviosispes FROM elabrev.unidadeordenadora codentidadesispes 
                     WHERE unicod = '{$unicod}'");
?>

<form name="formulario" id="formulario" method="POST" action="">
    <input type="hidden" id="requisicao" name="requisicao" value="" />

    <div class="div_info">  </div>

    <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <th style="background-color: #A9A9A9; font-size: 12px; text-align: left; width: 40%; ">UO: <?php echo $unicod . ' - ' . $unidade_decricao; ?> </th>
            <th style="background-color: #A9A9A9; font-size: 14px;">Projeto Esplanada Sustentável</th>
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
            <th colspan="2" style="font-size: 12px; text-align: left;">Meta de Economia a Pactuar (10%) - R$ <?php echo $dados['total'] ?> = a 10% do Total Global <?php echo $_SESSION['exercicio'] ?></th>
        </tr>
    </table>

    <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
        <tr>
            <th rowspan="2" width="45%">Despesa</th>
            <th rowspan="2" width="25%">Despesas Liquidadas 2012 </th>
            <th colspan="2" width="30%">Meta de Economia Proposta</th>
        </tr>
        <tr>
            <th width="12%">%</th>
            <th width="18%">Valor</th>
        </tr>
        <?php
        listaItensDespesas( $unicod );

        require_once APPRAIZ . "includes/workflow.php";

        $docid = buscarDocidEsplanada( $unicod );
        //ver($docid);
        
        $estado = pegaEstadoAtualEsplanada( $docid );
        ?>
    </table>

    <table style="width: 80%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="0" cellPadding=0 align="center">
        <th style="text-align: left;" colspan="2">Observações da UO:</th>
        <tr>
            <td>
            <?php echo campo_textarea( 'lcsobservacao', 'N', 'S', 'Observação ', 165, 2, 2000, '', '', '', '', 'id="lcsobservacao', $dados['lcsobservacao'], '' ); ?>
            </td>
        </tr>
        <tr>
            <td style="text-align: center;" colspan="2">
<?php
$perfil = array();
$perfil = pegaPerfilGeral( $_SESSION['usucpf'], $_SESSION['sisid'] );

if (( $estado == '' || $estado == WF_ES_EM_CADASTRAMENTO ) && ( in_array( UO_COORDENADOR_EQUIPE_TECNICA, $perfil ) || in_array( UO_EQUIPE_TECNICA, $perfil ) || in_array( PERFIL_DIRETOR_ADMIM, $perfil ) )) {
    ?>
                    <input type="button" id="salvar" class="salvar" name="salvar" value="Salvar">
                <?php } elseif ($estado == WF_ES_EM_VALIDACAO_DIRETOR && in_array( PERFIL_DIRETOR_ADMIM, $perfil )) { ?>
                    <input type="button" id="salvar" class="salvar" name="salvar" value="Salvar">
                <?php } elseif (in_array( PERFIL_SUPER_USUARIO, $perfil )) { ?>
                    <input type="button" id="salvar" class="salvar" name="salvar" value="Salvar">
                <?php } else { ?>
                    <input type="button" value="Salvar" disabled="disabled">
                <?php } ?>
                    <br />
                    <br />
            </td>
        </tr>
    </table>		
</form>

<?php if (in_array( PERFIL_SUPER_USUARIO, $perfil )): ?>
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
<?php echo notaRodaPe( $unicod ); ?>
        </td>
        <td width="50%" align="justify" style="border:thin; border-style:solid; border-color:#BEBEBE;">
            - Para fins de padronização, os montantes referentes às operações de cálculos efetuados acima estão expressos em duas casas decimas, com aproximação a partir de valores absolutos.
        </td>
    </tr>
</table>

<td>
<?php
$dados_wf = array("unicod" => $unicod, "esplaAluno" => 'N');
if ($docid != '') {
    wf_desenhaBarraNavegacao( $docid, $dados_wf );
}

//$tSql = "select * from elabrev.lancamentosustentavel order by lcsid asc";
//$rSet = $db->carregar($tSql);
//$i = 0;
//$pk = 1861;
//if ($rSet) {
//    foreach ($rSet as $row) {
//        
//        $lcsvalorempenhado = $row['lcsvalorempenhado'] ? $row['lcsvalorempenhado'] : 'null';
//        $lcsvalormeta = $row['lcsvalormeta'] ? $row['lcsvalormeta'] : 'null';
//        $lcsvalordeducao = $row['lcsvalordeducao'] ? $row['lcsvalordeducao'] : 'null';
//        
//        $iSql = "INSERT INTO elabrev.lancamentosustentavel
//                (lcsid,
//                dpsid,
//                entid, 
//                unicod, 
//                lcsvalorempenhado, 
//                lcsvalormeta, 
//                lcsvalordeducao, 
//                lcsstatus, 
//                lcsobservacao, 
//                anoexercicio) 
//                VALUES 
//                ($pk,
//                {$row['dpsid']},
//                null,
//                '{$row['unicod']}',
//                $lcsvalorempenhado,
//                $lcsvalormeta,
//                $lcsvalordeducao,
//                '{$row['lcsstatus']}',
//                'teste 123',
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
