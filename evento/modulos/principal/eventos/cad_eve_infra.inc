<?PHP

    if( !$_SESSION['evento']['eveid'] ){
        $db->sucesso( 'principal/inicioEvento', '', "Não é possivel acesar essa pagina, selecione um evento!");
    }
    $docid = evtCriarDoc();

    if( $_POST['ajaxitem'] ){

        $sql_porid = "SELECT porid FROM evento.porteevento WHERE {$_SESSION['evento']['publico']} BETWEEN porminpart AND pormaxpart";
        $porid = $db->pegaUm($sql_porid);

        $sql_coeid = "SELECT coeid FROM evento.empenho_unidade WHERE emuid = {$_SESSION['evento']['emuid']}";
        $coeid = $db->pegaUm($sql_coeid);

        if( $coeid == 3 || $coeid == 4 ){
            $porid = " pe.porid IN (1,2,3) ";
        }else{
            $porid = " pe.porid = {$porid} ";
        }

        $estuf = $_SESSION['evento']['estuf'];
        
	if( $_GET['tipo'] == 'fixo' ){
            #BUSCA A QUANTIDADE DISPONIVEL DE ITENS CONFORME CONTRATO E USA O iteid.
            $qtd_disponivel = quantidadeDisponivelItens( $_POST['ajaxitem'], $coeid );
            
            $sql = "
                SELECT  i.iteid,
                        iv.itvid,
                        iv.itvvalor,
                        umedescricao

                FROM evento.item AS i

                JOIN evento.unidademedida AS un ON un.umeid = i.umeid
                JOIN evento.itemvalor AS iv ON iv.iteid = i.iteid
                JOIN evento.contratopregao AS c ON c.coeid = iv.coeid
                JOIN evento.porteevento AS pe ON pe.porid = iv.porid
                JOIN evento.grupo AS g ON g.gruid = iv.gruid
                JOIN evento.grupoestado AS ge ON ge.gruid = iv.gruid --AND ge.gruid = {$gruid}

                WHERE c.coeid = {$coeid} AND ge.gruuf = '{$estuf}' AND {$porid} AND i.iteid = {$_POST['ajaxitem']};
            ";
            $rsAjax = $db->carregar( $sql );

            $resp = $rsAjax[0]['iteid'].'_'.$rsAjax[0]['itvid'].'_'.$rsAjax[0]['itvvalor'].'_'.$rsAjax[0]['umedescricao'].'_'.$qtd_disponivel;
            die( $resp );

	} elseif( $_GET['tipo'] == 'cadastrado' ) {
            #BUSCA DADOS ITENS.
            $sql = "
                 SELECT i.iteid,
                        iv.itvid,
                        iv.itvvalor,
                        umedescricao,
                        ( iq.iqtqtdmax - qtd_iv.quantidade_utilizada ) AS quantidade_utilizada

                FROM evento.item AS i

                JOIN evento.unidademedida AS un ON un.umeid = i.umeid
                JOIN evento.itemquantidade iq ON iq.iteid = i.iteid
                JOIN evento.itemvalor AS iv ON iv.iteid = i.iteid
                JOIN evento.contratopregao AS c ON c.coeid = iv.coeid
                JOIN evento.porteevento AS pe ON pe.porid = iv.porid
                JOIN evento.grupo AS g ON g.gruid = iv.gruid
                JOIN evento.grupoestado AS ge ON ge.gruid = iv.gruid
                
                JOIN(
                    SELECT itv.iteid, itv.coeid, sum(itc.itcquantidade) AS quantidade_utilizada 
                    FROM evento.itemvalor AS itv
                    JOIN evento.itemconsumo AS itc ON itc.itvid = itv.itvid
                    GROUP BY itv.iteid, itv.coeid
                ) AS qtd_iv ON qtd_iv.iteid = i.iteid AND qtd_iv.coeid = iq.coeid
                
                --jOIN (
                  --  SELECT itvid, sum(itcquantidade) as itcquantidade FROM evento.itemconsumo GROUP BY itvid
                --) AS qtd ON qtd.itvid = iv.itvid

                WHERE iq.coeid = {$coeid} AND ge.gruuf = '{$estuf}' AND {$porid} AND iv.itvid = {$_POST['ajaxitem']}
            ";
            $dados = $db->pegaLinha($sql);

            #BUSCA DADDOS DIAS
            $sql = "
                SELECT  itcid,
                        itvid,
                        eveid,
                        itcquantidade,
                        itcvalor,
                        itcdia,
                        itcdatainclusao,
                        itcparceria

                FROM evento.itemconsumo AS ic

                WHERE ic.eveid = {$_SESSION['evento']['eveid']} AND ic.itvid = {$_POST['ajaxitem']}
            ";
            $arrDias = $db->carregar( $sql );

            foreach( $arrDias as $data ){
                $arrQdtDia[] =  $data['itcquantidade'];
                $dias = implode(",", $arrQdtDia);
            }

            $resp = $dados['iteid'].'_'.$dados['itvid'].'_'.$dados['itvvalor'].'_'.$dados['umedescricao'].'_'.$dados['quantidade_utilizada'].'_'.$dias.'_'.$arrDias[0]['itcparceria'];
            echo $resp;
            die();
	}
    }

    if( $_POST['action'] == 'grava' ){
        salvaItensConsumidoEvento();
    }

    if( $_POST['action'] == 'excluir' ){
        excluirItemEvento( $_POST['itvid'] );
    }

    $titulo_modulo = "Eventos";

    #BUSCA DADOS CO EVENTO NECESSARIO PARA O FUNCIONAMENTO DA TELA.
    if( $_SESSION['evento']['eveid'] != ''){

        $eveid = $_SESSION['evento']['eveid'];

        $rsDadosEvento = buscaDadosCadEvento( $eveid );
    }

    #VERIFICA A PERMISSÃO DE EDITAR.
    $eventoPermissaoEdicao = eventoPermissaoEdicao();

    #CABEÇALHO.\
    include  APPRAIZ."includes/cabecalho.inc";
    monta_titulo( $titulo_modulo, 'Solicitar Pré-agendamento de eventos.' );
    echo'<br>';

    $res = array(
        0 => array("descricao" => "Lista", "id" => "4", "link" => "/evento/evento.php?modulo=inicio&acao=C&submod=evento"),
        1 => array ( "descricao" => "Informações Básicas", "id" => "4", "link" => "/evento/evento.php?modulo=principal/cadEvento&acao=A")
    );

    if( ( $rsDadosEvento['sevid'] != 1) && ( $rsDadosEvento['sevid'] != '') ){
	array_push(
            $res,
            array ("descricao" => "Documentos Anexos", "id" => "3", "link" => "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A"),
            array ("descricao" => "Infraestrutura", "id" => "2", "link" => "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A")
        );

	$pflcod = pegaPerfil( $_SESSION['usucpf'] );
    }

    if(mostraAbaDocOS($_SESSION['evento']['eveid'])){
	array_push(
            $res,
            array ("descricao" => "Ordem de Serviço", "id" => "7", "link" => "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A")
        );
    }

    if(mostraAbaDocPagamento($_SESSION['evento']['eveid'])){
	array_push(
            $res,
            array ("descricao" => "Documento de Pagamento", "id" => "5", "link" => "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A")
        );
    }

    if( ( $rsDadosEvento['sevid'] != 1) AND ( $rsDadosEvento['sevid'] != '') ){
	array_push(
            $res,
            array ("descricao" => "Avaliação", "id" => "0", "link" => "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A")
        );
    }

    echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A");

    headEvento($rsDadosEvento['evetitulo'],$rsDadosEvento['everespnome'], $rsDadosEvento['ungdsc'], $rsDadosEvento['ungcod'], $rsDadosEvento['eveurgente'], $rsDadosEvento['evedatainclusao'], true);

    echo '<br>';

    include APPRAIZ . 'includes/Agrupador.php';
    #BUSCA CONTRATO EVENTO.
    $sql_coeid = "SELECT coeid FROM evento.empenho_unidade WHERE emuid = {$_SESSION['evento']['emuid']}";
    $coeid = $db->pegaUm($sql_coeid);

    #MONTA A LISTAGEM DOS INTENS DO EVENTO.
    $sql = "
        SELECT  i.iteid as codigo,
                i.iteitem || ' - ' ||i.iteespecificacao as descricao,
                CAST( TRIM( REPLACE(i.iteitem, '.', '') ) AS integer ) AS nu_item
        FROM evento.item AS i
        LEFT JOIN evento.grupodeitens_contrato AS g ON g.gieid = i.gieid
        WHERE i.itestatus = 'A' AND iteidpai IS NULL AND g.coeid = {$coeid}
        ORDER BY nu_item ASC
    ";
    $rsComboItem = $db->carregar( $sql );

    foreach( $rsComboItem as $origem ){
	$i++;
	$destino[$i] = array("codigo" => $origem['codigo'], "descricao" => trim($origem['descricao'], 0));

	$sql = "
            SELECT  iteid as codigo,
                    iteitem ||' - '|| iteespecificacao as descricao,
                    iteidpai AS pai,
                    CAST( TRIM( REPLACE( iteitem, '.', '') ) AS integer) AS nu_item
            FROM evento.item AS i
            WHERE itestatus = 'A' AND iteidpai = '{$origem['codigo']}'
            ORDER BY nu_item ASC
        ";
        $rsComboItemFilho = $db->carregar( $sql );

	foreach($rsComboItemFilho as $filho){
            $i++;
            $destino[$i] = array("codigo" => $filho['codigo'], "descricao" => $filho['descricao']);

            $sql = "
                SELECT  iteid as codigo,
                        iteitem ||' - '|| iteespecificacao as descricao,
                        iteidpai AS pai,
                        CAST( TRIM( REPLACE( iteitem, '.', '') ) AS integer) as nu_item
                FROM evento.item AS i
                WHERE itestatus = 'A' AND iteidpai = '{$filho['codigo']}'
                ORDER BY nu_item ASC
            ";
            $rsComboItemNeto = $db->carregar( $sql );

            if($rsComboItemNeto){
                foreach($rsComboItemNeto as $neto){
                    $i++;
                    $destino[$i] = array("codigo" => $neto['codigo'], "descricao" => $neto['descricao']);
                }
            }
	}
    }
?>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>

<script type="text/javascript">

    function carregaItemdoEvento( id , tipo){

        limparCampos();

 	var hid_valoritem   = document.getElementById('hid_valoritem');
 	var tipoitemevento  = document.getElementById('tipoitemevento');
 	var lb_valor_item   = document.getElementById('lb_valor_item');
 	var unidadeMedida   = document.getElementById('unidadeMedida');
        var qtd_max         = document.getElementById('itcquantidade');

 	var req = new Ajax.Request(
                'evento.php?modulo=principal/eventos/cad_eve_infra&acao=A&tipo='+tipo,{
                method : 'post',
                parameters : '&ajaxitem=' + id,
                onComplete : function (res){
                    arrResponse = res.responseText.split("_");

                    var parceria = document.getElementById('itcparceria');

                    if(arrResponse[6] == 't'){
                        parceria.checked = 'checked';
                    } else {
                        parceria.checked = '';
                    }

                    if( tipo == 'cadastrado'){
                        var tdDias = '';
                        var arrDias = arrResponse[5].split(",");
                        var nDias = Number( document.getElementById('nDias').value );

                        tdDias  = "<table width=\"250\" border='0'> ";
                        tdDias += "<tr><td> Dia: </td><td> Qtd: </td><td> Valor: </td></tr>";

                        for( var i = 1; i <= nDias; i++){
                            if( !arrDias[i-1] ){
                                arrDias[i-1] = 0;
                            }
                            tdDias += "<tr> <td width=\"25\" > Dia "+i+" </td>";
                            tdDias += "<td width=\"25\"> <input type=\"text\" size=\"4\" class=\"normal\"name=\"qtd["+i+"]\" id=\"qtd["+i+"]\" value="+arrDias[i-1]+" onkeyup=\"this.value=mascaraglobal('###############',this.value); calcula(this.value, '"+i+"');\"> </td>";
                            tdDias += "<td id=\"colunaValor["+i+"]\" width=\"25\">";
                            tdDias += "<input type=\"hidden\" name=\"hid_valoritem\" id=\"hid_valoritem\" value=\"\"> </td></tr>";
                        }

                        tdDias += "<tr><td width=\"25\" > <b>Total</b> </td> <td width=\"25\"> </td> <td id=\"colunaValorTotal\" width=\"25\" ></td> </tr>";
                        tdDias += "</table>";

                        var tb_dias = document.getElementById('tb_dias');
                        tb_dias.innerHTML = tdDias;
                    }

                    var iteid           = arrResponse[0];
                    var itvid           = arrResponse[1];
                    var valor_item      = arrResponse[2];
                    var umedescricao 	= arrResponse[3];
                    var itcquantidade   = arrResponse[4];

                    document.getElementById('itvid').value = itvid;

                    hid_valoritem.value = valor_item;

                    qtd_max.value = '<b>'+itcquantidade+'</b>';
                    qtd_max.innerHTML = '<b>'+itcquantidade+'</b>';

                    unidadeMedida.innerHTML = '<b>'+umedescricao+'</b>';

                    lb_valor_item.value = '<b> R$: '+float2moeda(valor_item)+'</b>';
                    lb_valor_item.innerHTML = '<b> R$: '+float2moeda(valor_item)+'</b>';

                    tipoitemevento.value = iteid;

                    calculaTudo();
                }
        });
    }

    function calcula( qtd, dia ){
        var numCampos = Number( document.getElementById('nDias').value );
        var valorUnitario = Number( document.getElementById('hid_valoritem').value );
        var valorDia = '';

        for( var i = 1; i <= numCampos; i++ ){
            valorDia = Number( valorUnitario * Number(qtd) );

            if( isNaN( valorDia ) ){
                colunaValor = document.getElementById('colunaValor['+dia+']');
                colunaValor.innerHTML = 0;
            } else {
                colunaValor = document.getElementById('colunaValor['+dia+']');
                colunaValor.innerHTML = float2moeda(valorDia);
            }
	 }
	 calculaTotal(numCampos);
    }

    function calculaTudo(){
        var numCampos = Number( document.getElementById('nDias').value );
        var qtd = 0;
	var valorUnitario = Number( document.getElementById('hid_valoritem').value );
        var valorDia = '';

        for( var i= 1; i<= numCampos; i++ ){
            var qtd = document.getElementById('qtd['+i+']').value;
            valorDia    = Number( valorUnitario * Number(qtd) );

            if( isNaN( valorDia ) ){
                colunaValor = document.getElementById('colunaValor['+i+']');
                colunaValor.innerHTML = 0;
            } else {
                colunaValor = document.getElementById('colunaValor['+i+']');
                colunaValor.innerHTML = float2moeda(valorDia);
            }
	 }
	 calculaTotal(numCampos);
    }

    function calculaTotal(numCampos){
        var colunaValorTotal= document.getElementById('colunaValorTotal');
        var valor_total     = document.getElementById('valor_total_itens');
        var total_itens     = document.getElementById('qtd_total_itens');

        var valorTotal = 0;
        var valorParcial = 0;
        var qtd_total = 0;

        for( var i= 1; i<= numCampos; i++ ){
            colunaValue  = document.getElementById('colunaValor['+i+']');
            qtd_item    = document.getElementById('qtd['+i+']').value;
            valorParcial = colunaValue.innerHTML;
            valorParcial = valorParcial.replace(".","");
            valorParcial = valorParcial.replace(",",".");
            valorTotal  += Number( valorParcial );
            qtd_total += Number( qtd_item );
        }

        total_itens.value = qtd_total;
        valor_total.value = float2moeda(valorTotal);

        colunaValorTotal.innerHTML = float2moeda(valorTotal);
    }

    function float2moeda(num){
        x = 0;
        if(num<0) {
            num = Math.abs(num);
            x = 1;
        }
        if(isNaN(num)){
            num = "0";
        }
        cents = Math.floor((num*100+0.5)%100);
        num = Math.floor((num*100+0.5)/100).toString();

        if(cents < 10){
            cents = "0" + cents;
        }

        for (var i = 0; i < Math.floor((num.length-(1+i))/3); i++){
            num = num.substring(0,num.length-(4*i+3)) + '.' + num.substring(num.length-(4*i+3));
        }
        ret = num + ',' + cents;
        if(x == 1){
            ret = ' - ' + ret;
        }
        return ret;
    }

    function excluirItemEvento( itvid ){
        if(confirm("Deseja excluir este item?")){
            if( itvid != ''){
                document.getElementById('itvid').value = itvid;
                document.getElementById('action').value = 'excluir';
                document.formulario.submit();
            }
        }
    }

    function movimentoComteclaagrup(tecla, tipo, local, campoOrigem, campoDestino){
        if(window.event){ // Se IE
            codigoTecla = tecla.keyCode;
        }else if(tecla.which){
            codigoTecla = tecla.which;
        }

        campo = local;
        campoBusca 	= document.getElementById( 'busca'+campoOrigem );
        charTecla 	= String.fromCharCode(codigoTecla);
        agrupador 	= document.getElementById(campoOrigem);
        selecao 	= document.getElementById(campoOrigem).selectedIndex;

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
            moveSelectedOptions( agrupador, '' , true, '' );
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

    function limparCampos(){
        var numCampos           = Number( document.getElementById('nDias').value );
        var itcparceria         = document.getElementById('itcparceria');

        itcparceria.checked     = false;

 	for( var i = 1; i<= numCampos; i++){
            document.getElementById('qtd['+i+']').value = '';
            document.getElementById('qtd['+i+']').innerHTML = '';
 	}
    }

    function limitaConteudoAgrupadoragrup(busca, nomeAgrupador, tecla, nomeAgrupador2){
        var selectAgrupador = document.getElementById(nomeAgrupador);
        var busca = busca.value;

        busca = busca.toLowerCase();

        if(tecla.keyCode != 40 && tecla.keyCode != 38 && tecla.keyCode != 13){
            if(busca != ''){ // SE BUSCAR ALGO
                for (i = selectAgrupador.length - 1; i>=0; i--){
                    selectAgrupador.remove(i);
                }

                for (cont = 0; cont<= objsagrup.dados.texto.length; cont++){
                    if(objsagrup.dados.texto[cont]){
                        banco = objsagrup.dados.texto[cont].toLowerCase();

                        if(banco.indexOf(busca) != '-1'){
                            var opcoes   = document.createElement('option');
                            opcoes.text  = objsagrup.dados.texto[cont];
                            opcoes.value = objsagrup.dados.valor[cont];
                            try{
                                selectAgrupador.add(opcoes,null);
                            }catch(ex){
                                selectAgrupador.add(opcoes); // IE only
                            }
                        }
                    }
                }
            }else{
                for (i = selectAgrupador.length - 1; i>=0; i--) {
                    selectAgrupador.remove(i);
                }

                for (cont = 0; cont< objsagrup.dados.texto.length; cont++){
                    var opcoes   = document.createElement('option');
                    opcoes.text  = objsagrup.dados.texto[cont];
                    opcoes.value = objsagrup.dados.valor[cont];
                    try{
                        selectAgrupador.add(opcoes,null);
                    }catch(ex){
                        selectAgrupador.add(opcoes); // IE only
                    }
                }
            }
        }
    }

    function validaForm(tipo){
        var nDias = document.getElementById('nDias').value;
        var tipoitemevento  = document.getElementById('tipoitemevento');
        var itvid           = document.getElementById('itvid');
        var erro = 0;

	if( tipo == 'grava' ){

            if( tipoitemevento.value == '' || itvid.value == '' ){
                alert('Você deve selecionar um ítem de evento.');
                erro = 1;
                return false;
            }

            for( var i = 1; i <= nDias; i++){
                var dia  = document.getElementById('qtd['+i+']');

                if( dia.value == '' ){
                    alert('O campo "Dia '+i+'" é obrigatorio, é necessário atribuir um valor, pode ser 0 a quantidade desejada!');
                    erro = 1;
                    return false;
                }
            }
        }

        if( erro == 0 ){
            document.getElementById('action').value = tipo;
            document.formulario.submit();
	}
    }
</script>

<form name = "formulario" id="formulario" action="" method="POST">
    <input type="hidden" name="hid_valoritem" id="hid_valoritem" value="">
    <input type="hidden" name="itvid" id="itvid" value= "">
    <input type="hidden" name="nDias" id="nDias" value="<?=$rsDadosEvento['evequantidadedias'];?>">
    <input type="hidden" name="valor_total_itens" id="valor_total_itens" value="">
    <input type="hidden" name="qtd_total_itens" id="qtd_total_itens" value="">

    <table class="tabela Listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class ="SubTituloDireita" align="right" >Busca:</td>
            <td>
                <input id="buscatipoitemevento" name="buscatipoitemevento" class="normal" style="width: 650px;" onkeydown="return movimentoComteclaagrup(event, '', this ,'tipoitemevento', '');" onkeyup="limitaConteudoAgrupadoragrup(this,'tipoitemevento', event,'' );" onfocus="this.value = '';"  type="text" title=""  value="" style="width: 200px;" />
            <br>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right" >Selecione um ítem de evento:</td>
            <td>
                <select id="tipoitemevento" name="tipoitemevento[]" size="15" style="width:650px;" onclick="carregaItemdoEvento(this.value, 'fixo');"  class="combo campoEstilo">
                    <?php
                        foreach($destino as $option){
                            echo "<option value='{$option['codigo']}'> {$option['descricao']} </option>";
                        }
                    ?>
                </select>
            </td>
            <td>
                <!-- FILTRA OS ITENS DE ACORDO COM O ESCRITO NO INPUT TEXT -->
                <script type="text/javascript">
                    limitarQuantidade( document.getElementById( 'tipoitemevento' ));

                    var objsagrup = new Object;
                    objsagrup.dados = {
                        valor : new Array(),
                        texto : new Array()
                    };

                    var agrupadoragrup = document.getElementById('tipoitemevento');
                    for (cont=0; cont < agrupadoragrup.length; cont++){
                        objsagrup.dados.valor[cont] = agrupadoragrup[cont].value;
                        objsagrup.dados.texto[cont] = agrupadoragrup[cont].text;
                    }
                </script>
                <!-- FILTRA OS ITENS DE ACORDO COM O ESCRITO NO INPUT TEXT -->

                <div id="itemAdd"></div>
            </td>
            <td rowspan="5" style="background-color:#fafafa; color:#404040; width:1%" valign="top" align="left">
                <?php
                    wf_desenhaBarraNavegacao( $docid , array( '' => '' ) );
                ?>
            </td>
        </tr>

        <tr>
            <td class ="SubTituloDireita" align="right">Qauntidade Disponível: </td>
             <td id="qtd_disponiv">  <label name="itcquantidade" id="itcquantidade"> </label> </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Unidade de Medida: </td>
            <td id="unidadeMedida"> </td>
        </tr>

        <tr>
            <td class ="SubTituloDireita" align="right"> Valor: </td>
            <td id="tipoValor">
                <label name="lb_valor_item" id="lb_valor_item"> </label>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Parceria: </td>
            <td><input type="checkbox" id="itcparceria" name="itcparceria"></td>
        </tr>
        <tr>
            <td class ="subTituloDireita" > Dias: </td>
            <td>
                <div id="tb_dias">
                    <table width="250" border='0'>
                        <tr>
                            <td  width="30"> <b>Dia:</b> </td>
                            <td  width="25"> <b>Qtd:</b> </td>
                            <td  width="25"> <b>Valor</b> </td>
           		</tr>
           		<?PHP
                            $nDias = $rsDadosEvento['evequantidadedias'];
                            for( $i = 1; $i <= $nDias; $i++) {
           		?>
           		<tr>
                            <td> Dia <?=$i;?> </td>
                            <td>
                                <?php
                                    echo "
                                        <input type=\"text\" size=\"4\" class=\"normal\"name=\"qtd[$i]\" onkeyup=\"this.value=mascaraglobal('###############',this.value); calcula(this.value, '$i');\" id=\"qtd[$i]\" value=''>
                                    ";
                                ?>
                            </td>
                            <td id="colunaValor[<?=$i;?>]">
                                <input type="hidden" name="hid_valoritem" id="hid_valoritem" value="">
                            </td>
                        </tr>
           		<?PHP } ?>
           		<tr>
                            <td> <b>Total</b> </td>
                            <td> </td>
                            <td id="colunaValorTotal"> </td>
                        </tr>
                    </table>
                </div>
            </td>
        </tr>

        <?  if( !temPerfilEmpresa() ){ ?>
                <tr>
                    <td  class ="SubTituloDireita" align="right"> </td>
                    <td>
                       <input type="hidden" name="action" value="0" id="action">
                       <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="validaForm('grava');">
                       <input type="button" name="btnLimpar" value="Novo ítem" id="btnLimpar" onclick="limparCampos();">
                    </td>
                </tr>
    </table>
</form>

<?PHP
            }

    $estado = pegaEstadoAtualEvento();

    if( $estado == EM_CADASTRAMENTO_WF ){
        $acao = "
            <img src=\"/imagens/alterar.gif\" border=\"0\" style=\"cursor: pointer\" title=\"Alterar\" onclick=\"carregaItemdoEvento('|| ic.itvid ||', ''cadastrado'');\">
            <img src=\"/imagens/excluir.gif\" border=\"0\" style=\"cursor: pointer\" title=\"Excluir\" onclick=\"excluirItemEvento('|| ic.itvid ||');\">
        ";
    }else{
        $acao = "
            <img src=\"/imagens/alterar_01.gif\" border=\"0\" title=\"Alterar\">
            <img src=\"/imagens/excluir_01.gif\" border=\"0\" title=\"Excluir\">
        ";
    }

    $sql = "
        SELECT  '{$acao}',
                i.iteitem || '.' AS iteitem,
                i.iteespecificacao,
                '<spam style=\"color:#0066cc;\">' || trim( to_char(ic.itcvalor, '999G999G999G990D99') ) || '</spam>' AS itcvalor,
                ' '||sum( ic.itcquantidade )|| ' ' AS itcquantidade,
                sum ( (ic.itcquantidade * ic.itcvalor) ) AS valor_total,
                qt.qtd_dias || ' Dia(s)' as n_dias,
                CASE WHEN itcparceria = TRUE
                        THEN '<img src=\"/imagens/adm_eventos/like_ok.png\" width=\"26px\" title=\"Tem Parceria\" border=\"0\">'
                        ELSE ''
                END AS itcparceria
        FROM evento.itemconsumo AS ic

        JOIN evento.itemvalor AS iv ON iv.itvid = ic.itvid

        JOIN evento.item AS i ON i.iteid = iv.iteid

        JOIN(
            SELECT itvid, COUNT(itcid) AS qtd_dias
            FROM evento.itemconsumo
            WHERE (itcquantidade > 0) AND eveid = {$_SESSION['evento']['eveid']}
            GROUP BY itvid
        ) AS qt On qt.itvid = ic.itvid

        WHERE eveid = {$_SESSION['evento']['eveid']}

        GROUP BY ic.itvid, i.iteitem, i.iteespecificacao, ic.itcvalor, n_dias, itcparceria
        ORDER BY i.iteespecificacao
    ";
    $cabecalho = array( "Ação", "Ítem", "Descrição", "Valor Unitario", "QTD", "Valor Total", "Nº de Dias", "Parceria");

    $alinhamento = Array('center','right','','right','center','right','center','center');
    $tamanho = Array('4%','5%','50%','10%','5%','10%','5%','5%');
    $db->monta_lista($sql, $cabecalho, 100, 10, "S", "center", "S", "", $tamanho, $alinhamento);
    
/*
EXEMPLO:														
    $param['ordena'] 	   = (true ou false);																
    $param['totalLinhas']  = (true ou false);												
    $param['managerOrder'] = array( 12 => 'o.obrnome',
                                    15 => 'oc.ocrdtinicioexecucao',
                                    16 => 'oc.ocrdtterminoexecucao',
                                    18 => array('campo' => "DATE_PART('days', NOW() - o.obrdtultvistoria)", 'alias' => "obrdtultvistoria"),
                                    19 => 'obrdtultvistoria'
    $param['classTable'] = 'lista listagem listaClica'
                                                                       ); 												
    $db->monta_lista($sql, $cabecalho, 50, 20, '', '100%', '', '', '', '', '', $param);
 */    
    
    
    
?>

<script>

    calculaTudo();

</script>

<?php
//verifica permissao edição
echo $eventoPermissaoEdicao;
?>