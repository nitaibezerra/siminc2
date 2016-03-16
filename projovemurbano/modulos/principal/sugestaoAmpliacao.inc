<?php
if( $_SESSION['projovemurbano']['ppuid'] == '2' && !$_SESSION['projovemurbano']['muncod'] ){
	include_once 'sugestaoAmpliacao2013.inc';
}elseif($_SESSION['projovemurbano']['ppuid'] == '3' && !$_SESSION['projovemurbano']['muncod'] ){
	include_once 'sugestaoAmpliacao2014.inc';
}else{

    if ($_POST['requisicao'] == 'salvaDados'){

        if (is_array($_POST['mtpvalor']) && $_POST['mtpvalor']) {
            $sql = '';
            $suaid = $_REQUEST['suaid'];
            $suaverdade = $_REQUEST['suaverdade'] == 'sim' ? 'TRUE' : 'FALSE';
            if ($suaid) {
                $sql .= "delete from projovemurbano.metasdoprograma where ppuid = {$_POST['ppuid']} and suaid = {$suaid} and tpmid in (2,3,5,6,8,9,11,12,14,15);";
                $sql .= "update projovemurbano.sugestaoampliacao set suaverdade = $suaverdade where ppuid = {$_SESSION['projovemurbano']['ppuid']} and pjuid = {$_SESSION['projovemurbano']['pjuid']};";
            } else {
                $sqlSugestao = "
                    insert into projovemurbano.sugestaoampliacao (pjuid, suaverdade, suametasugerida, suastatus, suametaajustada, ppuid)
                            values ({$_SESSION['projovemurbano']['pjuid']}, $suaverdade, null, 'A', null, {$_SESSION['projovemurbano']['ppuid']}) returning suaid;
                ";
                $suaid = $db->pegaUm($sqlSugestao);
            }

            foreach ($_POST['mtpvalor'] as $k => $v) {
                $v = $v ? $v : '0';
                if (in_array($k, array(2, 3))) {
                    switch ($k) {
                        case 2: // sugerida
                            $sql .= " update projovemurbano.sugestaoampliacao set suametasugerida = {$v} where suaid = {$suaid} and ppuid = {$_SESSION['projovemurbano']['ppuid']};";
                            $sql .= " insert into projovemurbano.metasdoprograma (tpmid, ppuid, suaid, cmeid, mtpvalor) values ({$k}, {$_SESSION['projovemurbano']['ppuid']}, {$suaid}, null, {$v}); ";
                            break;
                        case 3: // ajustada
                            $sql .= " update projovemurbano.sugestaoampliacao set suametaajustada = {$v} where suaid = {$suaid} and ppuid = {$_SESSION['projovemurbano']['ppuid']};";
                            $sql .= " insert into projovemurbano.metasdoprograma (tpmid, ppuid, suaid, cmeid, mtpvalor) values ({$k}, {$_SESSION['projovemurbano']['ppuid']}, {$suaid}, null, {$v}); ";
                            break;
                    }                    
                }elseif (in_array($k, array(5, 6))) {
                    switch ($k) {
                        case 5: // sugerida
                            $sql .= " update projovemurbano.sugestaoampliacao set suametasugerida = {$v} where suaid = {$suaid} and ppuid = {$_SESSION['projovemurbano']['ppuid']};";
                            break;
                        case 6: // ajustada
                            $sql .= " update projovemurbano.sugestaoampliacao set suametaajustada = {$v} where suaid = {$suaid} and ppuid = {$_SESSION['projovemurbano']['ppuid']};";
                            break;
                    }
                } else {
                    if (is_numeric($v)) {
                        $sql .= "
                            insert into projovemurbano.metasdoprograma (tpmid, ppuid, suaid, cmeid, mtpvalor)
                                    values ({$k}, {$_SESSION['projovemurbano']['ppuid']}, {$suaid}, null, {$v});
                        ";
                    }
                }
            }
            if ($sql) {
                $db->executar($sql);
                $db->commit();
                $db->sucesso('principal/termoAdesaoAjustado', '');
            }
        }
        die;
    }

    if ($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }

    include_once APPRAIZ . 'includes/cabecalho.inc';
    echo '<br>';

    echo montarAbasArray(montaMenuProJovemUrbano(), $_SERVER['REQUEST_URI']);

    monta_titulo('Projovem Urbano', montaTituloEstMun());

    $sql = "SELECT * FROM projovemurbano.sugestaoampliacao WHERE pjuid='" . $_SESSION['projovemurbano']['pjuid'] . "' and ppuid = {$_SESSION['projovemurbano']['ppuid']}";
    $sugestaoampliacao = $db->pegaLinha($sql);

    if ($sugestaoampliacao) {
        $requisicao = "atualizarSugestaoAmpliacao";
        extract($sugestaoampliacao);
    } else {
        $requisicao = "inserirSugestaoAmpliacao";
    }

    $habilita = 'S';
    $disabilita = '';
    $perfil = pegaPerfilGeral();
    if (in_array(PFL_CONSULTA, $perfil)) {
        $habilita = 'N';
        $disabilita = 'S';
    }

    if( $db->testa_superuser() || $habilita == 'S' ){
        $habilitaMetaSugerida = 'S';
    }else{
        $habilitaMetaSugerida = 'N';
    }
    
    if ($_SESSION['projovemurbano']['muncod']) {
        $cmecodibge = $_SESSION['projovemurbano']['muncod'];
    } else {
        $cmecodibge = $db->pegaUm("select estcod from territorios.estado where estuf = '{$_SESSION['projovemurbano']['estuf']}'");
    }

    $sql = "select * from projovemurbano.cargameta where cmecodibge = '{$cmecodibge}' and ppuid = {$_SESSION['projovemurbano']['ppuid']}";
    $rsCargaMeta = $db->pegaLinha($sql);
    
    if($sugestaoampliacao['suaid'] != ''){
        $total_meta = $sugestaoampliacao['suametasugerida'];
    }else{
        $total_meta = $rsCargaMeta['cmemeta'];
    }
?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
    
    //Faz a verificação do perfil e desabilita o radio button
    jQuery(document).ready(function() {
        
        if(parseInt($('[name=ppuid]').val()) === 2){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=verificaMetaDestinada&suaid=<?=$sugestaoampliacao['suaid'];?>&metaDestinada=sugerida",
                async   : false,
                success: function(resp){
                    if(resp === '8'){
                        $('#metaDestinada_sugerida_J').attr('checked',true);
                    }else{
                        $('#metaDestinada_sugerida_P').attr('checked',true);
                    }
                }
            });

            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=verificaMetaDestinada&suaid=<?=$sugestaoampliacao['suaid'];?>&metaDestinada=ajustada",
                async   : false,
                success: function(resp){
                    if(resp === '9'){
                        $('#metaDestinada_ajustada_J').attr('checked',true);
                    }else{
                        $('#metaDestinada_ajustada_P').attr('checked',true);
                    }
                }
            });
        }
        
        $('#suaverdade').click(function() {
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=verificaMetaDestinada&cmeid=<?=$rsCargaMeta['cmeid'];?>&metaDestinada=atendida",
                async   : false,
                success: function(resp){
                    if(resp === '7'){
                        $('#metaDestinada_sugerida_J').attr('checked',true);
                    }else{
                        $('#metaDestinada_sugerida_P').attr('checked',true);
                    }
                }
            });
        });

        $('[name=suaverdade]').click(function() {
            if (this.value == 'sim') {
                $('.tr_sugestao_ampliacao').show();
            } else {
                $('.tr_sugestao_ampliacao').hide();
            }
        });

        $('#btnSalvar').click(function() { //ppuid
 
            suaverdade = $('[name=suaverdade]:checked').val();
            
            if( (parseInt($('[name=muncod]').val()) > 0) ){
                salvarSugestaoMetaMunicipio();
            }else{
                salvarSugestaoMetaUF();
            }

            
        });
        
        <?php if ($disabilita == 'S') { ?>
                    Disabilitado(this);
        <?php } ?>
            
       
    });

    function Disabilitado() {
        jQuery("input[name='suaverdade']").each(function(i) {
            jQuery(this).attr('disabled', 'disabled');
        });
    }

    function salvarSugestaoMetaMunicipio() {
        //VERIFICA ANO 2012
        if(parseInt($('[name=ppuid]').val()) == 1){ 
            var suaverdade = jQuery("[name^='suaverdade']:checked");
            if (suaverdade.length == 0) {
                alert('Marque uma "Sugestão de ampliação de meta"');
                return false;
            }
            if ( $('#suaverdade').val() == "sim" ) {
                if (jQuery('#suametasugerida').val() == "") {
                    alert('Preencha uma meta sugerida');
                    return false;
                }
            }
            if( $('#suametaajustada').length ){
                if( $('#suametaajustada').val() == "" ){
                    alert('Preencha a meta ajustada');
                    return false;
                }
            }
            $('#form').submit();
        }

        //VERIFICA ANO 2013
        if(parseInt($('[name=ppuid]').val()) == 2){
            var suaverdade = jQuery("[name^='suaverdade']:checked");
            if (suaverdade.length == 0) {
                alert('Marque uma "Sugestão de ampliação de meta"');
                return false;
            }

            if ( $('#suaverdade').val() == "sim" ) {
                if (jQuery('#suametasugerida').val() == "") {
                    alert('Preencha uma meta sugerida');
                    return false;
                }
                if($("[name^='metaDestinada_sugerida']:checked").val() == ""){
                    alert('Selecione Meta sugerida destinada');
                    return false;
                }
            }

            if( $('#suametaajustada').length ){

                if( $('#suametaajustada').val() == "" ){
                    alert('Preencha a meta ajustada');
                    return false;
                }
                if($("[name^='metaDestinada_ajustada']:checked").val() == ""){
                    alert('Selecione Meta Ajustada destinada');
                    return false;
                }
            }
            $('#form').submit();
        }   

      //VERIFICA ANO 2014
        if(parseInt($('[name=ppuid]').val()) == 3){
        	alert('Olha!');
        	die;
            var suaverdade = jQuery("[name^='suaverdade']:checked");
            if (suaverdade.length == 0) {
                alert('Marque uma "Sugestão de ampliação de meta"');
                return false;
            }

            if ( $('#suaverdade').val() == "sim" ) {
                if (jQuery('#suametasugerida').val() == "") {
                    alert('Preencha uma meta sugerida');
                    return false;
                }
                if($("[name^='metaDestinada_sugerida']:checked").val() == ""){
                    alert('Selecione Meta sugerida destinada');
                    return false;
                }
            }

            if( $('#suametaajustada').length ){

                if( $('#suametaajustada').val() == "" ){
                    alert('Preencha a meta ajustada');
                    return false;
                }
                if($("[name^='metaDestinada_ajustada']:checked").val() == ""){
                    alert('Selecione Meta Ajustada destinada');
                    return false;
                }
            }
            $('#form').submit();
        }          
    }
    
    function salvarSugestaoMetaUF(){
    	if( parseInt( $('[name=ppuid]').val() ) == 1 ){
            if(!suaverdade){
                alert('O campo "Sugestão de ampliação de meta" é obrigatório!');
                $('[name=suaverdade]').focus();
                return false;
            }
            
            if(suaverdade == 'sim'){
                erro = 0;
                $.each($("[name^='mtpvalor']"), function(i, v) {
                    id = $(v).attr('name').replace('mtpvalor[', '').replace(']', '');
                    if( $(v).val() == '' && (id == 2 || id == 3) ){
                        erro++;
                        $(v).focus();
                        return false;
                    }
                });

                if(erro > 0){
                    alert('Preencha todas as metas!');
                    return false;
                }
                $('[name=requisicao]').val('salvaDados');
            }
            $('#form').submit();                
        }
        
        if( parseInt( $('[name=ppuid]').val() ) == 2 ){
            if(!suaverdade){
                alert('O campo "Sugestão de ampliação de meta" é obrigatório!');
                $('[name=suaverdade]').focus();
                return false;
            }
            
            if(suaverdade == 'sim'){
                erro = 0;
                $.each($("[name^='mtpvalor']"), function(i, v) {
                    //id = $(v).attr('name').replace('mtpvalor[', '').replace(']', '');
                    if ($(v).val() == '' ){//&& (id == 5 || id == 8 || id == 11 || id == 14)) {
                        erro++;
                        $(v).focus();
                        return false;
                    }
                });

                if(erro > 0){
                    alert('Preencha todas as metas Sugeridas/Ajustadas!');
                    return false;
                }

                if (!verificarValoresMetas()) {
                    alert('O total das metas deve ser igual a Meta Total');
                    return false;
                }
                $('[name=requisicao]').val('salvaDados');
            }
            $('#form').submit();
        }
  
    if( parseInt( $('[name=ppuid]').val() ) == 3 ){
        if(!suaverdade){
            alert('O campo "Sugestão de ampliação de meta" é obrigatório!');
            $('[name=suaverdade]').focus();
            return false;
        }
        
        if(suaverdade == 'sim'){
            erro = 0;
            $.each($("[name^='mtpvalor']"), function(i, v) {
                //id = $(v).attr('name').replace('mtpvalor[', '').replace(']', '');
                if ($(v).val() == '' ){//&& (id == 5 || id == 8 || id == 11 || id == 14)) {
                    erro++;
                    $(v).focus();
                    return false;
                }
            });

            if(erro > 0){
                alert('Preencha todas as metas Sugeridas/Ajustadas!');
                return false;
            }

            if (!verificarValoresMetas()) {
                alert('O total das metas deve ser igual a Meta Total');
                return false;
            }
            
            $('[name=requisicao]').val('salvaDados');
        }
        $('#form').submit();
    }
}

    function verificarValoresMetas(){
        totalajustado = $("[name=mtpvalor[6]]").val();
        totalsugerido = $("[name=mtpvalor[5]]").val();
        ajustado = 0;
        sugerido = 0;

        $.each($("[name^='mtpvalor']"), function(i, v) {

            id = $(v).attr('name').replace('mtpvalor[', '').replace(']', '');

            if (id == 8 || id == 11 || id == 14) {
                if ($(v).val())
                    sugerido = parseInt(sugerido) + parseInt($(v).val());
            }
            if (id == 9 || id == 12 || id == 15) {
                $(v).val()
                ajustado = parseInt(ajustado) + parseInt($(v).val());
            }

        });

        if (totalajustado != ajustado && $('[name=mtpvalor[6]]').val()) {
            alert('O total das metas ajustadas deve ser igual a Meta Total Ajustada!');
            return false;
        }

        if (totalsugerido != sugerido) {
            alert('O total das metas sugeridas deve ser igual a Meta Total Sugerida!');
            return false;
        }
        return true;
    }
    function sugestaoAmpliacaoMeta(obj) {
        if (obj.value == "sim") {
            document.getElementById('tr_metasugerida').style.display = '';
            document.getElementById('tr_metaajustada_radio').style.display = '';
            if (document.getElementById('tr_metaajustada'))
                document.getElementById('tr_metaajustada').style.display = '';
            
            if (document.getElementById('tr_metaajustada_radio'))
                document.getElementById('tr_metaajustada_radio').style.display = '';
        }
        if (obj.value == "nao") {
            document.getElementById('tr_metasugerida').style.display = 'none';
            if (document.getElementById('tr_metaajustada'))
                document.getElementById('tr_metaajustada').style.display = 'none';
        }
    }
</script>

<?php

    if ($_SESSION['projovemurbano']['muncod']) {
        $cmecodibge = "AND muncod = '" . $_SESSION['projovemurbano']['muncod'] . "'";
    } else {
        $cmecodibge = "AND estuf = '" . $_SESSION['projovemurbano']['estuf'] . "'"; //" and estuf = '".$db->pegaUm("select estuf from territorios.estado where estuf = '{$_SESSION['projovemurbano']['estuf']}'")."'";
    }

    $sql = "
        SELECT a.suaid, a.pjuid, a.suaverdade, cast(a.suametasugerida as integer) as suametasugerida, a.suastatus, cast(a.suametaajustada as integer) as suametaajustada, a.ppuid
        FROM projovemurbano.projovemurbano p
        JOIN projovemurbano.sugestaoampliacao a on a.pjuid = p.pjuid 
        WHERE p.ppuid = {$_SESSION['projovemurbano']['ppuid']} AND 
        p.pjuid = {$_SESSION['projovemurbano']['pjuid']}
        {$cmecodibge}
    ";
    $rsSugestaoMeta = $db->pegaLinha($sql);
?>

<form id="form" name="form" method="POST">
    <input type="hidden" name="requisicao" value="<?= $requisicao; ?>">
    <input type="hidden" name="suaid" value="<?= $rsSugestaoMeta['suaid']; ?>">
    <input type="hidden" name="estuf" value="<?= $_SESSION['projovemurbano']['estuf']; ?>">
    <input type="hidden" name="muncod" value="<?= $_SESSION['projovemurbano']['muncod']; ?>">
    <input type="hidden" name="ppuid" id="ppuid" value="<?= $_SESSION['projovemurbano']['ppuid']; ?>">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
        <tr>
            <td class="SubTituloDireita">Orientações:</td>
            <td>A sugestão de meta é importante para a análise pela SECADI, possibilitando ou não a ampliação da meta original.</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Sugestão de ampliação de meta:</td>
            <td>
                <input type="radio" id="suaverdade" name="suaverdade" value="sim" onclick="sugestaoAmpliacaoMeta(this);" <?= ($suaverdade == "t" ? "checked" : ""); ?> > Sim 
                <input type="radio" id="suaverdade" name="suaverdade" value="nao" onclick="sugestaoAmpliacaoMeta(this);" <?= ($suaverdade != "t") ? "checked" : ""; ?> > Não
            </td>
        </tr>
        <?php
        if ($_SESSION['projovemurbano']['estuf']):
            $sql = "Select  t.tpmid, mtpvalor, tpmdescricao
                      From projovemurbano.tipometadoprograma t
                      Left Join projovemurbano.metasdoprograma m on m.tpmid = t.tpmid and cmeid = {$rsCargaMeta['cmeid']} and ppuid = {$_SESSION['projovemurbano']['ppuid']}
                      Where t.tpmid in (7,10,13)";
            $rsMetas = $db->carregar($sql);

            if ($rsMetas) {
                foreach ($rsMetas as $dados) {
                    switch ($dados['tpmid']) {
                        case 7: $juventudeViva = $dados['mtpvalor']; break;
                        case 10: $prisional = $dados['mtpvalor']; break;
                        case 13: $publicogeral = $dados['mtpvalor']; break;
                    }
                }
            }

            $rsSugestaoMeta['suaid'] = $rsSugestaoMeta['suaid'] ? $rsSugestaoMeta['suaid'] : 'NULL';

            if( $_SESSION['projovemurbano']['ppuid'] == '1' ){
                $where = "where t.tpmid in (2, 3)";
            }elseif( $_SESSION['projovemurbano']['ppuid'] == '2' && !$rsSugestaoMeta['suaid']){
                $where = "where t.tpmid in (5, 8, 11, 14)";
            }elseif( $_SESSION['projovemurbano']['ppuid'] == '2' && $rsSugestaoMeta['suaid'] ){
                $where = "where t.tpmid in (5, 6, 8, 9, 11, 12, 14, 15)";
            }else{
                $where = "where t.tpmid in (5, 6)";
            }

            $sql = "select  t.tpmid,mtpvalor,tpmdescricao
                      from projovemurbano.tipometadoprograma t
                      left join projovemurbano.metasdoprograma m on m.tpmid = t.tpmid and suaid = {$rsSugestaoMeta['suaid']} and ppuid = {$_SESSION['projovemurbano']['ppuid']}
                       {$where}";
            $rsMetasAjustadas = $db->carregar($sql);

      		foreach ($rsMetasAjustadas as $meta): ?>        
            <tr class="tr_sugestao_ampliacao" style="display:none;">
                <td class="subtituloDireita"><?php echo $meta['tpmdescricao']; ?></td>
                <td>
                <?php $valor = '';
                if ($meta['tpmid'] == 2) {
                    $valor = $meta['mtpvalor'] ? $meta['mtpvalor'] : $rsCargaMeta['cmemeta'];
                }elseif ($meta['tpmid'] == 3) {
                    $valor = $meta['mtpvalor'] ? $meta['mtpvalor'] : '';
                }elseif ($meta['tpmid'] == 5) {
                    $valor = $rsSugestaoMeta['suametasugerida'] != "" ? $rsSugestaoMeta['suametasugerida'] : $rsCargaMeta['cmemeta'];
                } elseif ($meta['tpmid'] == 6) {
                    $valor = $rsSugestaoMeta['suametaajustada'] != "" ? $rsSugestaoMeta['suametaajustada'] : '';
                } elseif (in_array($meta['tpmid'], array(8))) { // 8,9
                    $valor = $meta['mtpvalor'] ? $meta['mtpvalor'] : $juventudeViva;
                } elseif (in_array($meta['tpmid'], array(9))) { // 8,9
                    $valor = $meta['mtpvalor'];
                } elseif (in_array($meta['tpmid'], array(11))) { // 11,12
                    $valor = $meta['mtpvalor'] ? $meta['mtpvalor'] : $prisional;
                } elseif (in_array($meta['tpmid'], array(12))) { // 11,12
                    $valor = $meta['mtpvalor'];
                } elseif (in_array($meta['tpmid'], array(14))) { // 14,15
                    $valor = $meta['mtpvalor'] ? $meta['mtpvalor'] : $publicogeral;
                } elseif (in_array($meta['tpmid'], array(15))) { // 14,15
                    $valor = $meta['mtpvalor'];
                }

                $habil = "S";
                if (in_array($meta['tpmid'], array(6, 9, 12, 15)) && in_array(PFL_SECRETARIO_ESTADUAL, $perfil)) {
                    $habil = 'N';
                }

                echo campo_texto('mtpvalor['.$meta['tpmid'].']', 'S', $habil, '', 25, '', '######', '', '', '', '', '', '', $valor);
                unset($valor);
                ?>
                </td>
            </tr>
            <?php endforeach;
        else: ?>
        <?php
        if($rsSugestaoMeta){ 
	        $sql = "select tpmdescricao
	        from projovemurbano.tipometadoprograma t
	        inner join projovemurbano.metasdoprograma m on m.tpmid = t.tpmid and suaid = {$rsSugestaoMeta['suaid']} and ppuid = {$_SESSION['projovemurbano']['ppuid']}
	        where t.tpmid in (7, 8) and mtpvalor > 0";
	        $testaMetasJ = $db->carregar($sql);
	    
	        $sql = "select tpmdescricao
	        from projovemurbano.tipometadoprograma t
	        inner join projovemurbano.metasdoprograma m on m.tpmid = t.tpmid and suaid = {$rsSugestaoMeta['suaid']} and ppuid = {$_SESSION['projovemurbano']['ppuid']}
	        where t.tpmid in (13,14) and mtpvalor > 0";
	        $testaMetasP = $db->carregar($sql);
	        
	        
	        $sql = "select tpmdescricao
	        from projovemurbano.tipometadoprograma t
	        inner join projovemurbano.metasdoprograma m on m.tpmid = t.tpmid and suaid = {$rsSugestaoMeta['suaid']} and ppuid = {$_SESSION['projovemurbano']['ppuid']}
	        		where t.tpmid in ( 9) and mtpvalor > 0";
	        		$testaMetasAjustadasJ = $db->carregar($sql);
	        		//         ver($testaMetasJ);
	        
	        $sql = "select tpmdescricao
	        from projovemurbano.tipometadoprograma t
	        inner join projovemurbano.metasdoprograma m on m.tpmid = t.tpmid and suaid = {$rsSugestaoMeta['suaid']} and ppuid = {$_SESSION['projovemurbano']['ppuid']}
	        where t.tpmid in (15) and mtpvalor > 0";
	        $testaMetasAjustadasP = $db->carregar($sql);
        }
//         ver( $testaMetasJ, $testaMetasP, $testaMetasAjustadasJ, $testaMetasAjustadasP);
        ?>
            <tr class="tr_sugestao_ampliacao" <?= (($suaid && $suaverdade == "t") ? "" : "style=\"display:none\"") ?> id="tr_metasugerida">
                <td class="SubTituloDireita">Meta sugerida:</td>
                <td>
                    <?php echo campo_texto('suametasugerida', 'S', $habilitaMetaSugerida, 'Meta sugerida:', 10, 8, "########", '', '', '', 0, 'id="suametasugerida"', '', $total_meta); ?>
                    Estudantes
                </td>
            </tr>
        <? if( $_SESSION['projovemurbano']['ppuid'] == '2'  || $_SESSION['projovemurbano']['ppuid'] == '3'): ?>
            <tr class="tr_sugestao_ampliacao" <?= (($suaid && $suaverdade == "t") ? "" : "style=\"display:none\"") ?> id="tr_metasugerida">
                <td class="SubTituloDireita">Meta sugerida destinada:</td>
                <td>
                <?php 
               $arViva = array(7, 8, 9);
               $arPublic = array(13, 14, 15);
                ?>
                    <input type="radio" id="metaDestinada_sugerida_J" name="metaDestinada_sugerida" <?= ( is_array( $testaMetasJ ) ? "checked" : ""); ?> value="J"> Juventude Viva 
                    <input type="radio" id="metaDestinada_sugerida_P" name="metaDestinada_sugerida" <?= ( is_array( $testaMetasP ) ? "checked" : ""); ?> value="P"> Público Geral
                </td>
            </tr>
        <? endif;
        if (in_array(PFL_ADMINISTRADOR, $perfil) || in_array(PFL_EQUIPE_MEC, $perfil) || $db->testa_superuser()):
            if ($sugestaoampliacao): ?>
            <tr <?= (($suaid && $suaverdade == "t") ? "" : "style=\"display:none\"") ?> id="tr_metaajustada">
                <td class="SubTituloDireita">Meta ajustada:</td>
                <td>
                    <?php echo campo_texto('suametaajustada', 'S', $habilita, 'Meta ajustada', 10, 8, "########", "", '', '', 0, 'id="suametaajustada"', ''); ?> 
                    Estudantes
                </td>
            </tr>
                <? if( $_SESSION['projovemurbano']['ppuid'] == '2'|| $_SESSION['projovemurbano']['ppuid'] == '3'): ?>
            <tr <?= (($suaid && $suaverdade == "t") ? "" : "style=\"display:none\"") ?> id="tr_metaajustada_radio">
                <td class="SubTituloDireita">Meta Ajustada destinada:</td>
                <td>
                    <input type="radio" id="metaDestinada_ajustada_J" name="metaDestinada_ajustada" <?= ( is_array( $testaMetasAjustadasJ ) ? "checked" : ""); ?> value="J"> Juventude Viva 
                    <input type="radio" id="metaDestinada_ajustada_P" name="metaDestinada_ajustada" <?= ( is_array( $testaMetasAjustadasP ) ? "checked" : ""); ?> value="P"> Público Geral
                </td>
            </tr>
                <? endif;
            endif; 
        endif;
    endif; ?>
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    <input type="submit" class="salvar" value="Salvar">
                </td>
            </tr>
    </table>
</form>

<? registarUltimoAcesso(); ?>
<?php if ($suaverdade == "t"): ?>
    <script>
        $('.tr_sugestao_ampliacao').show();
    </script>
<?php endif; 
	}
?>