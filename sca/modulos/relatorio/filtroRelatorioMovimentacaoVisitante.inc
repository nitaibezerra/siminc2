<?php

// salva os POST na tabela
if ( $_REQUEST['salvar'] == 1 ){
    $existe_rel = 0;
    $sql = sprintf(
        "select prtid from public.parametros_tela where prtdsc = '%s'",
        $_REQUEST['titulo']
    );
    $_REQUEST['dtInicio'] =  formata_data_sql($_REQUEST['dtInicio']);
    $_REQUEST['dtFinal'] =  formata_data_sql($_REQUEST['dtFinal']);
    $existe_rel = $db->pegaUm( $sql );
    if ($existe_rel > 0)
    {
        $sql = sprintf(
            "UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d WHERE prtid = %d",
            $_REQUEST['titulo'],
            addslashes( addslashes( serialize( $_REQUEST ) ) ),
            $_SESSION['usucpf'],
            $_SESSION['mnuid'],
            $existe_rel
        );
        $db->executar( $sql );
        $db->commit();
    }
    else
    {
        $sql = sprintf(
            "INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid ) VALUES ( '%s', '%s', %s, '%s', %d )",
            $_REQUEST['titulo'],
            addslashes( addslashes( serialize( $_REQUEST ) ) ),
            'FALSE',
            $_SESSION['usucpf'],
            $_SESSION['mnuid']
        );
        $db->executar( $sql );
        $db->commit();
    }
    ?>
    <script type="text/javascript">
        alert('Operação realizada com sucesso!');
        location.href = '?modulo=relatorio/filtroRelatorioMovimentacaoVisitante&acao=A';
    </script>
    <?
    die;
}

// remove consulta
if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
    $sql = sprintf(
        "DELETE from public.parametros_tela WHERE prtid = %d",
        $_REQUEST['prtid']
    );
    $db->executar( $sql );
    $db->commit();
    ?>
        <script type="text/javascript">
            location.href = '?modulo=relatorio/filtroRelatorioMovimentacaoVisitante&acao=A';
        </script>
    <?
    die;
}
// FIM remove consulta

// exibe consulta
if ( isset( $_REQUEST['form'] ) == true && !$_REQUEST['carregar']){


    if (!empty($_REQUEST['prtid'])){

        $sql = sprintf(    "select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
        $itens = $db->pegaUm( $sql );

        $dados = unserialize( stripslashes( stripslashes( $itens ) ) );

        if(!empty($dados)){
            $_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
            unset( $_REQUEST['salvar'] );

            $_REQUEST['dtInicio'] = formata_data($_REQUEST['dtInicio']);
            $_REQUEST['dtFinal'] = formata_data($_REQUEST['dtFinal']);
        }


    }

    include "relatorioMovimentacaoVisitante.inc";
    exit;

}

// remove flag de submissão de formulário
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] )
{
    $carregarAux = $_REQUEST['carregar'];
    unset( $_REQUEST['form'] );
}
// FIM remove flag de submissão de formulário



// carrega consulta do banco
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){

    $sql = sprintf(    "select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid'] );
    $itens = $db->pegaUm( $sql );
    $dados = unserialize( stripslashes( stripslashes( $itens ) ) );
    extract( $dados );
    $_REQUEST = $dados;
    unset( $_REQUEST['form'] );
    unset( $_REQUEST['pesquisa'] );
    $titulo = $_REQUEST['titulo'];
    $agrupador2 = array();

    if ( $_REQUEST['agrupador'] ){

        foreach ( $_REQUEST['agrupador'] as $agrupadorCarregado ){
            $valorAgrupador = explode(".", $agrupadorCarregado);
            $itemAgrupador['codigo'] = $agrupadorCarregado;
            switch ($valorAgrupador[1]) {
                case "vttdoc":
                         $itemAgrupador['descricao'] = "Nº do Documento";
                         break;
                     case "tiddsc":
                         $itemAgrupador['descricao'] = "Tipo Documento";
                         break;
                     case "vttnome":
                         $itemAgrupador['descricao'] = "Nome";
                         break;
                     case "vstnumcracha":
                         $itemAgrupador['descricao'] = "Crachá";
                         break;
                     case "vtttel":
                         $itemAgrupador['descricao'] = "Telefone";
                         break;
                     case "vstdatentrada":
                         $itemAgrupador['descricao'] = "Entrada";
                         break;
                     case "vstdatsaida":
                         $itemAgrupador['descricao'] = "Saída";
                         break;
                     case "dstdsc":
                         $itemAgrupador['descricao'] = "Destino";
                         break;
                     case "usucpfentrada":
                         $itemAgrupador['descricao'] = "Quem Autorizou";
                         break;
                     case "usunome":
                         $itemAgrupador['descricao'] = "Quem Autorizou";
                         break;
            }
               $agrupador2[] = $itemAgrupador;
        }



    }

}

// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

monta_titulo('Relatório de Movimentação de Visitantes','&nbsp;');

?>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" >

    /**
     * Alterar visibilidade de um bloco.
     *
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco( bloco ){
        var div_on = document.getElementById( bloco + '_div_filtros_on' );
        var div_off = document.getElementById( bloco + '_div_filtros_off' );
        var img = document.getElementById( bloco + '_img' );
        var input = document.getElementById( bloco + '_flag' );
        if ( div_on.style.display == 'none' )
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }

    function exibeRelatorio( tipo ){

        var formulario = document.formulario;
        var agrupador  = document.getElementById( 'agrupador' );


        // Tipo de relatorio
        formulario.pesquisa.value='1';

        prepara_formulario();


        if ( tipo == 'salvar' ){


            selectAllOptions( agrupador );

            if ( formulario.titulo.value == '' ) {
                alert( 'É necessário informar a descrição do relatório!' );
                formulario.titulo.focus();
                return;
            }

            var nomesExistentes = new Array();

            <?php
                $sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela WHERE usucpf = '".$_SESSION['usucpf']."' AND mnuid = ".$_SESSION['mnuid'];
                $nomesExistentes = $db->carregar( $sqlNomesConsulta );
                if ( $nomesExistentes ){
                    foreach ( $nomesExistentes as $linhaNome )
                    {
                        print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
                    }
                }
            ?>

            var confirma = true;
            var i, j = nomesExistentes.length;
            for ( i = 0; i < j; i++ ){
                if ( nomesExistentes[i] == formulario.titulo.value ){
                    confirma = confirm( 'Deseja alterar a consulta já existente?' );
                    break;
                }
            }
            if ( !confirma ){
                return;
            }

            formulario.target = '_self';
            formulario.action = 'sca.php?modulo=relatorio/filtroRelatorioMovimentacaoVisitante&acao=A&salvar=1';
            formulario.submit();

        }else if ( tipo == 'relatorio' ){

            formulario.action = 'sca.php?modulo=relatorio/filtroRelatorioMovimentacaoVisitante&acao=A';
            window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
            formulario.target = 'relatorio';
            formulario.submit();

        } else {

            var formulario = document.formulario;
            var agrupador  = document.getElementById( 'agrupador' );


            if ( !agrupador.options.length ){
                alert( 'Favor selecionar ao menos um item para agrupar o resultado! \n (Agrupador de resultado)' );
                return false;
            }

            selectAllOptions( agrupador );

            //valida data
            if(formulario.dtInicio.value != '' && formulario.dtFinal.value != ''){
                if(!validaData(formulario.dtInicio)){
                    alert("Data Inicial Inválida.");
                    formulario.dtInicio.focus();
                    return false;
                }
                if(!validaData(formulario.dtFinal)){
                    alert("Data Final Inválida.");
                    formulario.dtFinal.focus();
                    return false;
                }
            }

            formulario.target = 'Resultado';
            var janela = window.open( '?modulo=relatorio/filtroRelatorioMovimentacaoVisitante&acao=A', 'Resultado', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );

            formulario.submit();
            janela.focus();

        }

    }

    function excluirRelatorio(prtid){
        if(confirm('Deseja realmente excluir este relatório?')){
            document.formulario.excluir.value = '1';
            document.formulario.prtid.value = prtid;
            document.formulario.target = '_self';
            document.formulario.submit();
        }
    }

    function carregarConsulta(prtid){
        document.formulario.carregar.value = '1';
        document.formulario.prtid.value = prtid;
        document.formulario.target = '_self';
        document.formulario.submit();
    }

    function carregarRelatorio(prtid){
        document.formulario.prtid.value = prtid;
        exibeRelatorio('relatorio');
    }

    function consultaTela(){

        var dtInicio = document.formulario.dtInicio.value;
        var dtFinal =  document.formulario.dtFinal.value;

        // campos obrigatorios
        if(dtInicio == '' || dtFinal == ''){

            alert('Preencha um período para a geração do relatório.')

        }else{

            // verificando periodo da geracao do relatorio
            var obDt = new Data();
            var dtTermino = obDt.dtAddDia(dtInicio, <?php echo SCA_PERIODO_RELMOVVISITANTE ?>);

            if(obDt.comparaData(dtFinal, dtTermino, '>')){
                alert('O período da consulta não pode ser maior que <?php echo SCA_PERIODO_RELMOVVISITANTE ?> dias');
                return false;
            }else{

                document.formulario.prtid.value = '';
                exibeRelatorio();
            }

        }


    }

</script>
<form name="formulario" id="formulario" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td class="SubTituloDireita">Título</td>
            <td colspan="2">
                <?= campo_texto( 'titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Seleção dos campos para o relatório: <br/>(Agrupador de resultado)</td>
            <td colspan="2">
                <?php
                	// HTML DO AGRUPADOR
                	
                $html_agrupador =
<<<EOF
	<table>
		<tr>
			<td colspan=4>
				<input id="busca{NOME_ORIGEM}" name="busca{NOME_ORIGEM}" class="normal" onkeydown="return movimentoComtecla{NOME_ORIGEM}(event, '', this ,'{NOME_ORIGEM}', '{NOME_DESTINO}');" onkeyup="limitaConteudoAgrupador{NOME_ORIGEM}(this,'{NOME_ORIGEM}', event,'{NOME_DESTINO}' );"  type="text" title=""  value="Pesquisar campo..." style="width: 200px;" />
			</td>
		</tr>
		<tr valign="middle">
			<td>
				<select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="7"  onkeydown="return movimentoComtecla{NOME_ORIGEM}(event, 'envia', this , '{NOME_ORIGEM}', '{NOME_DESTINO}');" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
			</td>
			<td>
				<img src="../imagens/rarrow_one.gif" style="padding: 2px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
				<img src="../imagens/rarrow_all.gif" style="padding: 2px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
				<img src="../imagens/larrow_all.gif" style="padding: 2px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
				<img src="../imagens/larrow_one.gif" style="padding: 2px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
			</td>
			<td>
				<select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="7" onkeydown="return movimentoComtecla{NOME_ORIGEM}(event, 'retorna', this ,  '{NOME_ORIGEM}', '{NOME_DESTINO}');" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
			</td>
			<td>
				<img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
				<img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
			</td>
		</tr>
	</table>
	<script type="text/javascript" language="javascript">
		limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
		limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
		{POVOAR_ORIGEM}
		{POVOAR_DESTINO}
		sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
		// TECLAS : 13 - enter  |  38 - para cima   |  40 - para baixo
                
		function movimentoComtecla{NOME_ORIGEM}(tecla, tipo, local, campoOrigem, campoDestino){
    		
    		var codigoTecla = null;
			if(window.event){ // Se IE
				codigoTecla = tecla.keyCode;
			}else if(tecla.which){
				codigoTecla = tecla.which;
			}
    		
			var campo = local;
			var campoBusca 	= document.getElementById( 'busca'+campoOrigem );
			var charTecla 	= String.fromCharCode(codigoTecla);
			var agrupador 	= document.getElementById(campoOrigem);
			var destino 	= document.getElementById(campoDestino);
			var selecao 	= document.getElementById(campoOrigem).selectedIndex;
    		
				if(tecla.keyCode == 40){ // para baixo
					if(agrupador.selectedIndex == -1){
						agrupador[0].selected = true;
					}else{
						for (i=0;i < agrupador.length; i++){
							if(agrupador[i].selected == true) {
								if(agrupador[i+1] != null){
									agrupador[i].selected = false;
									agrupador[i+1].selected = true;
									break;
								}else{
									agrupador[i].selected = false;
									break;
								}
							}
						}
					}
				}else if (tecla.keyCode == 13){ // tecla enter
					moveSelectedOptions( agrupador, destino , true, '' );
				}else if(tecla.keyCode == 38){ // para cima
					if(agrupador.selectedIndex == -1){
						for (i=0;i < agrupador.length;i++){
								agrupador[i].selected = true;
								break;
						}
					}else{
						for (i=agrupador.length;i > 0;i--){
							if(agrupador[i] != null ){
								if(agrupador[i].selected == true) {
									if(agrupador[i-1] != null ){
										agrupador[i].selected = false;
										agrupador[i-1].selected = true;
										break;
									}else{
										break;
									}
								}
							}
						}
					}
			
				}
			return true;
		}
                
		var objs{NOME_ORIGEM} 	= new Object;
			objs{NOME_ORIGEM}.dados = {
						valor : new Array(),
						texto : new Array()
				  	 };
                
		var agrupador{NOME_ORIGEM} =  document.getElementById('{NOME_ORIGEM}');
		for (cont=0; cont < agrupador{NOME_ORIGEM}.length; cont++){
			objs{NOME_ORIGEM}.dados.valor[cont] = agrupador{NOME_ORIGEM}[cont].value;
			objs{NOME_ORIGEM}.dados.texto[cont] = agrupador{NOME_ORIGEM}[cont].text;
		}
                
		function limitaConteudoAgrupador{NOME_ORIGEM}(busca, nomeAgrupador, tecla, nomeAgrupador2){
			var selectAgrupador 		= document.getElementById(nomeAgrupador);
			var selectAgrupadorDestino 	= document.getElementById(nomeAgrupador2);
			var busca 					= busca.value;
			busca = busca.toLowerCase();
			if(tecla.keyCode != 40 && tecla.keyCode != 38 && tecla.keyCode != 13){
				if(busca != ''){ // SE BUSCAR ALGO
					for (i = selectAgrupador.length - 1; i>=0; i--) {
		      			selectAgrupador.remove(i);
					}
					for (var cont = 0; cont < objs{NOME_ORIGEM}.dados.texto.length; cont++){
						var banco = objs{NOME_ORIGEM}.dados.texto[cont].toLowerCase();
						if(banco.indexOf(busca) != '-1'){
							var opcoes   = document.createElement('option');
							opcoes.text  = objs{NOME_ORIGEM}.dados.texto[cont];
							opcoes.value = objs{NOME_ORIGEM}.dados.valor[cont];
							try{
								selectAgrupador.add(opcoes,null);
							}catch(ex){
								selectAgrupador.add(opcoes); // IE only
							}
							for (i = selectAgrupadorDestino.length - 1; i>=0; i--) {
		      					if(selectAgrupadorDestino[i].text == objs{NOME_ORIGEM}.dados.texto[cont]){
		      						for (x = selectAgrupador.length - 1; x>=0; x--) {
		      							if(selectAgrupador[x].text == selectAgrupadorDestino[i].text){
		      								selectAgrupador.remove(x);
		      							}
		      						}
		      					}
							}
						}
					}
				}else{
					for (i = selectAgrupador.length - 1; i>=0; i--) {
		      			selectAgrupador.remove(i);
					}
					for (cont = 0; cont< objs{NOME_ORIGEM}.dados.texto.length; cont++){
						var opcoes   = document.createElement('option');
						opcoes.text  = objs{NOME_ORIGEM}.dados.texto[cont];
						opcoes.value = objs{NOME_ORIGEM}.dados.valor[cont];
						try{
							selectAgrupador.add(opcoes,null);
						}catch(ex){
							selectAgrupador.add(opcoes); // IE only
						}
					}
					for (i = selectAgrupadorDestino.length - 1; i>=0; i--) {
						for (x = selectAgrupador.length - 1; x>=0; x--) {
		      				if(selectAgrupadorDestino[i].text == selectAgrupador[x].text){
		      					selectAgrupador.remove(x);
		      				}
		      			}
					}
				}
			}
		}
	</script>
EOF;

                    // Início dos agrupadores
                    $agrupador = new Agrupador('formulario',$html_agrupador);


                    // Dados padrão de destino (nulo)
                    if($agrupador2){
                        $destinoAgrp = $agrupador2;
                    }

                    // Dados padrão de origem
                    $origem = array(
                        'cracha' => array(
                            'codigo'    => 'visita.vstnumcracha',
                            'descricao' => 'Crachá'
                        ),
                        'destino.dstdsc' => array(
                            'codigo'    => 'destino.dstdsc',
                            'descricao' => 'Destino'
                        ),
                        'documento' => array(
                            'codigo'    => 'visitante.vttdoc',
                            'descricao' => 'Documento'
                        ),
                        'visita.vstdatentrada' => array(
                            'codigo'    => 'visita.vstdatentrada',
                            'descricao' => 'Entrada'
                        ),
                        'nome' => array(
                            'codigo'    => 'visitante.vttnome',
                            'descricao' => 'Nome'
                        ),
                        'quemAutorizou' => array(
                            'codigo'    => 'usuario.usunome',
                            'descricao' => 'Quem Autorizou'
                        ),
                        'visita.vstdatsaida' => array(
                            'codigo'    => 'visita.vstdatsaida',
                            'descricao' => 'Saída'
                        ),
                        'telefone' => array(
                            'codigo'    => 'visitante.vtttel',
                            'descricao' => 'Telefone'
                        ),
                        'tipoDocumento' => array(
                            'codigo'    => 'tipodocumento.tiddsc',
                            'descricao' => 'Tipo Documento'
                        )

                    );

                    // exibe agrupador
                    $agrupador->setOrigem( 'naoAgrupador', null, $origem );
                    $agrupador->setDestino( 'agrupador', null, $destinoAgrp );
                    $agrupador->exibir();
                ?>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data Inicial:</td>
            <td style="width: 120px;">
                <?= campo_data2( 'dtInicio', 'S', 'S', '', 'S' ); ?>
            </td>
            <td>
                <?= campo_texto( 'hrInicial', 'N', 'S', '', 5, 5, '##:##', '', '', '', '', '', '', '00:00'); ?>
                Horas
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data Final:</td>
            <td style="width: 120px;">
                <?= campo_data2( 'dtFinal', 'S', 'S', '', 'S' ); ?>
            </td>
            <td>
                <?= campo_texto( 'hrFinal', 'N', 'S', '', 5, 5, '##:##', '', '', '', '', '', '', '23:59'); ?>
                Horas
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Visitante:</td>
            <td colspan="2">
                <?= campo_texto( 'nomeVisitante', 'N', 'S', '', 30, 30, '', ''); ?>
                <input type="checkbox" id="chkExibeFoto" name="chkExibeFoto" <?=($chkExibeFoto=="on")?"CHECKED":"";?>>
                <span onclick="selecionaCampoPorID('chkExibeFoto')">
                    Exibir Foto?
                </span>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Destino:</td>
            <td colspan="2">
                <?= campo_texto( 'destino', 'N', 'S', '', 30, 30, '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Filtros para o relatório:</td>
            <td colspan="2">
                <input type="checkbox" id="chkVisitanteSemRegistroSaida" name="chkVisitanteSemRegistroSaida" <?=($chkVisitanteSemRegistroSaida=="on")?"CHECKED":"";?>>
                <span onclick="selecionaCampoPorID('chkVisitanteSemRegistroSaida')">
                    Visitantes que não registraram a saída
                </span>
                <br/>
                <input type="checkbox" id="chkPermanenciaSuperior" name="chkPermanenciaSuperior" <?=($chkPermanenciaSuperior=="on")?"CHECKED":"";?>>
                <span onclick="selecionaCampoPorID('chkPermanenciaSuperior')">
                    Visitantes com permanência no MEC superior a um dia
                </span>
                <br/>
                <input type="checkbox" id="chkVisitanteSemDocumento" name="chkVisitanteSemDocumento" <?=($chkVisitanteSemDocumento=="on")?"CHECKED":"";?>>
                <span onclick="selecionaCampoPorID('chkVisitanteSemDocumento')">
                    Visitantes que entraram no MEC sem documento
                </span>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Ordenação:</td>
            <td colspan="2">
                <?php
                $arrayId = array("rbtDataHoraEntrada","rbtDataHoraSaida","rbtNomeVisitante","rbtDestino");
                $arrayValor = array("DHE","DHS","NDV","DES");
                $arrayRotulos = array("Data e Hora de Entrada","Data e Hora de Saída","Nome do Visitante","Destino");
                montaRadio("ordenacao", $arrayId, $arrayValor, $arrayRotulos, $arrayValor[0], 1);
                ?>
            </td>
        </tr>
        <tr>
            <td colspan="3" style="padding: 0px;" bgcolor="#DCDCDC" align="left" >
                <table class="tabela" align="center" width="100%" bgcolor="#DCDCDC" cellspacing="1" cellpadding="3" style="border-bottom: medium none; border-top: medium none; width: 100%;">
                    <tr>
                        <td onclick="javascript:onOffBloco( 'minhasconsultas' );">
                            <img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
                            Minhas Consultas
                            <input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
                        </td>
                    </tr>
                </table>
                <div id="minhasconsultas_div_filtros_off">
                </div>
                <div id="minhasconsultas_div_filtros_on" style="display:none;">
                    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;width: 100%;">
                            <tr>
                                <td class="SubTituloDireita" valign="top">Consultas</td>
                                <?php

                                    $sql = sprintf(
                                        "SELECT
                                             CASE WHEN usucpf = '{$_SESSION['usucpf']}' THEN
                                                 '<img align=\"absbottom\" border=\"0\" src=\"../imagens/preview.gif\" title=\" Exibir relatório \" onclick=\"carregarRelatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluirRelatorio(' || prtid || ');\">'
                                             END as acao,
                                            --'<div id=\"nome_' || prtid || '\">' || prtdsc || '</div>' as descricao
                                            '<a title=\" Carregar consulta no formulário \" href=\"javascript: carregarConsulta(' || prtid || ')\">' || prtdsc || '</a>' as descricao
                                         FROM
                                             public.parametros_tela
                                         WHERE
                                             mnuid = %d AND usucpf = '%s' ORDER BY descricao",
                                        $_SESSION['mnuid'],
                                        $_SESSION['usucpf']
                                    );
                                    //dbg($sql,1);
                                    $cabecalho = array('Ação', 'Nome');
                                ?>
                                <td>
                                    <?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, null, null, null ); ?>
                                </td>
                            </tr>
                    </table>
                </div>
            </td>
        </tr>
        <tr>
            <th></th>
            <th colspan="2" align="left">
                <input type="hidden" name="form" value="1"/>
                <input type="hidden" name="pesquisa" value="1"/>
                <input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
                <input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
                <input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
                <input type="button" value="Visualizar" onclick="consultaTela();" style="cursor: pointer;"/>
                <input type="button" value="Salvar Consulta" onclick="exibeRelatorio('salvar');" style="cursor: pointer;"/>
            </th>
        </tr>
    </table>
</form>