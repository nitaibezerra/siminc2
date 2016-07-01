<?php

    function montaSqlProfEquivalente($filtros = array()){
	// Filtros academico.portariaprofequival
	if($_GET['ano']) $arWherePpe[] = "ppe.ppeano = {$_GET['ano']}";
	//if($_GET['semestre']) $arWherePpe[] = "ppe.ppesemestre = {$_GET['semestre']}";
	if($_GET['portaria']) $arWherePpe[] = "ppe.ppeid = {$_GET['portaria']}";

	// Filtros academico.movprofequivalente
	if($_GET['ano']) $arWhereMpe[] = "mpe.mpeano = {$_GET['ano']}";
	if($_GET['mes']) $arWhereMpe[] = "mpe.mpemes = {$_GET['mes']}";
	if($_GET['portaria']) $arWhereMpe[] = "mpe.ppeid = {$_GET['portaria']}";

	// Filtros academico.portariavalor
	if($_GET['portaria']) $arWherePtv[] = "ptv.ppeid = {$_GET['portaria']}";

	// Calculo saldo
	$stCalculo = "coalesce((coalesce(ptvvalor,0)) - ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) ))";

	// Banco Eqv
	$stBancoEqv = "coalesce( ( (coalesce(mpevlr20h,0)*0.58) + (coalesce(mpevlr40h,0)*1) + (coalesce(mpevlrdedexclusiva,0)*1.7) + (coalesce(mpevlrsubstituto,0)*1) + (coalesce(mpevlrvisitante,0)*1.7) ),0)";

        $mpevlr20h_inici = "'<input type=\"text\" name=\"mpevlr20h[' || e.entid || ']\" id=\"mpevlr20h_' || e.entid || '\" value=\"' || ";
        $mpevlr20h_final = "|| '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>'";

	$sql = "
            SELECT  UPPER(entsig) as codigo,
                    UPPER(entnome) as orgao,
                    upper(mun.estuf) as uf,
                    ppe.ppenumero,
                    ppeano,
                    (select mesdsc from public.meses where mescod::integer='".$_GET['mes']."') as mes,

                    {$mpevlr20h_inici}
                    CASE WHEN mpevlr20h > 0
                        THEN trim( to_char( coalesce(mpevlr20h,0 ), '999G999G999G999D99' ) )
                        ELSE '0,00'
                    END
                    {$mpevlr20h_final} AS mpevlr20h,

                    '<input type=\"text\" name=\"mpevlr40h[' || e.entid || ']\" id=\"mpevlr40h_' || e.entid || '\" value=\"' ||
                    case when mpevlr40h > 0
                        then trim(to_char(coalesce(mpevlr40h,0), '999G999G999G999D99'))
                        else '0,00'
                    end
                    || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlr40h,

                    '<input type=\"text\" name=\"mpevlrdedexclusiva[' || e.entid || ']\" id=\"mpevlrdedexclusiva_' || e.entid || '\" value=\"' ||
                    case when mpevlrdedexclusiva > 0
                        then trim(to_char(coalesce(mpevlrdedexclusiva,0), '999G999G999G999D99'))
                        else '0,00'
                    end
                    || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrdedexclusiva,

                    '<input type=\"text\" name=\"mpevlrsubstituto[' || e.entid || ']\" id=\"mpevlrsubstituto_' || e.entid || '\" value=\"' ||
                    case when coalesce(mpevlrsubstituto,0) > 0
                        then trim(to_char(coalesce(mpevlrsubstituto,0), '999G999G999G999D99'))
                        else '0,00'
                    end
                    || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrsubstituto,

                    '<input type=\"text\" name=\"mpevlrvisitante[' || e.entid || ']\" id=\"mpevlrvisitante_' || e.entid || '\" value=\"' ||
                    case when coalesce(mpevlrvisitante,0) > 0
                        then trim(to_char(coalesce(mpevlrvisitante,0), '999G999G999G999D99'))
                        else '0,00'
                    end
                    || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'[###.]###,##\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrvisitante,

                    '<input type=\"text\" name=\"mpevlrvagos[' || e.entid || ']\" id=\"mpevlrvagos_' || e.entid || '\" value=\"' ||
                    case when coalesce(mpevlrvagos,0) > 0
                        then trim(to_char(coalesce(mpevlrvagos,0), '999999'))
                        else '0'
                    end
                    || '\" class=\"normal calculoTotal selecionaLinha\" size=\"6\" onKeyUp=\"this.value=mascaraglobal(\'######\',this.value);\" onfocus=\"MouseClick(this);this.select();\" onmouseout=\"MouseOut(this);\" onblur=\"MouseBlur(this);\"/>' as mpevlrvagos,


                    '<span id=\"total_' || e.entid || '\">' ||
                    case when coalesce((mpevlr20h+mpevlr40h+mpevlrdedexclusiva+mpevlrsubstituto+mpevlrvisitante),0) > 0
                        then trim(to_char(coalesce((mpevlr20h+mpevlr40h+mpevlrdedexclusiva+mpevlrsubstituto+mpevlrvisitante),0), '999G999G999G999D99'))
                        else '0,00'
                    end
                    || '</span>' as total,

                    '<span id=\"mpevlrbcequiv_' || e.entid || '\">' ||
                    case when {$stBancoEqv} > 0
                        then trim(to_char({$stBancoEqv}, '999G999G999G999D99'))
                        else '0,00'
                    end
                    || '</span>

                    <input type=\"hidden\" name=\"mpevlrbcequiv[' || e.entid || ']\" value=\"' ||
                    case when {$stBancoEqv} > 0
                        then trim(to_char({$stBancoEqv}, '999G999G999G999D99'))
                        else '0,00'
                    end
                    || '\" />

                    ' as mpevlrbcequiv,

                    '<span id=\"ptvvalor_' || e.entid || '\">' ||
                    case when ptvvalor > 0
                        then trim(to_char(coalesce(ptvvalor,0), '999G999G999G999D99'))
                        else '0,00'
                    end ||
                    '</span>' as ptvvalor,

                    '<span id=\"saldo_' || e.entid || '\">' ||
                    case when {$stCalculo} = 0
                        then '0,00'
                        else trim(to_char({$stCalculo}, '999G999G999G999D99'))
                    end ||

                    '</span> <input type=\"hidden\" name=\"entid[]\" value=\"' || e.entid || '\" /> <input type=\"hidden\" name=\"mpevlrsaldo[' || e.entid || ']\" value=\"' ||
                    case when {$stCalculo} = 0
                        then '0,00'
                        else trim(to_char({$stCalculo}, '999G999G999G999D99'))
                    end
                    || '\" />' as portmpmec

            FROM entidade.entidade e

            INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
            LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
            LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
            LEFT JOIN academico.portariavalor ptv ON ptv.entid = e.entid

            ".(is_array($arWherePtv) ? ' and '.implode(' and ', $arWherePtv) : '')."

            LEFT JOIN academico.portariaprofequival ppe ON 1=1 ".(is_array($arWherePpe) ? ' and '.implode(' and ', $arWherePpe) : '')."

            LEFT JOIN academico.movprofequivalente mpe ON mpe.ppeid = ppe.ppeid AND mpestatus = 'A' AND mpe.entid = e.entid ".(is_array($arWhereMpe) ? ' and '.implode(' and ', $arWhereMpe) : '')."

            WHERE e.entstatus = 'A' AND ef.funid  in ('12') ".(is_array($arWhere) ? ' and '.implode(' and ', $arWhere) : '')."

            ORDER BY e.entsig, e.entnome
        ";
	return $sql;
    }

    if($_REQUEST['requisicao'] == 'carregaComboPortarias'){
	$ano = $_REQUEST['ano'];
	//$semestre = $_REQUEST['mes'] <= 6 ? 1 : 2;

	$sql = "
            SELECT  ppeid as codigo,
                    ppenumero as descricao
            FROM academico.portariaprofequival

            WHERE ppeano = {$ano} --AND ppesemestre = {$semestre}
        ";

	$db->monta_combo('portaria', $sql, 'S', 'Selecione...', '', '', '', '', 'S');
	die;
    }

    if($_REQUEST['requisicao'] == 'salvaDados'){

	if(is_array($_POST['entid'])){

            $ppeid  = $_POST['portaria'];
            $mpeano = $_POST['ano'];
            $mpemes = $_POST['mes'];
            $sql    = "";

            if($ppeid && $mpeano && $mpemes){

                if(verificaTipoPesquisa($_POST) == 'A'){
                    $mpeprocesso = "'A'";
                } else {
                    $mpeprocesso = "'N'";
                }

                foreach($_POST['entid'] as $entid){

                    $mpeid = $db->pegaUm("select mpeid from academico.movprofequivalente where entid = {$entid} and ppeid = {$ppeid} and mpeano = {$mpeano} and mpemes = {$mpemes}");

                    $mpevlr20h          = desformata_valor($_POST['mpevlr20h'][$entid]);
                    $mpevlr40h 		= desformata_valor($_POST['mpevlr40h'][$entid]);
                    $mpevlrdedexclusiva = desformata_valor($_POST['mpevlrdedexclusiva'][$entid]);

                    $mpevlrsubstituto 	= desformata_valor($_POST['mpevlrsubstituto'][$entid]);
                    $mpevlrsub20h 	= desformata_valor($_POST['mpevlrsub20h'][$entid]);
                    $mpevlrsub40h 	= desformata_valor($_POST['mpevlrsub40h'][$entid]);

                    $mpevlrvisitante 	= desformata_valor($_POST['mpevlrvisitante'][$entid]);
                    //$mpevlrvagos 	= desformata_valor($_POST['mpevlrvagos'][$entid]);
                    $mpevlrvagos 	= $_POST['mpevlrvagos'][$entid];
                    $mpevlrbcequiv 	= desformata_valor($_POST['mpevlrbcequiv'][$entid]);
                    $mpevlrsaldo 	= desformata_valor($_POST['mpevlrsaldo'][$entid]);

                    if($mpeid){
                        $sql .= "
                            update academico.movprofequivalente set
                                    mpevlr20h           = {$mpevlr20h},
                                    mpevlr40h 		= {$mpevlr40h},
                                    mpevlrdedexclusiva 	= {$mpevlrdedexclusiva},
                                    mpevlrsubstituto 	= {$mpevlrsubstituto},
                                    mpevlrvisitante 	= {$mpevlrvisitante},
                                    mpevlrvagos 	= {$mpevlrvagos},
                                    mpevlrbcequiv 	= {$mpevlrbcequiv},
                                    mpevlrsaldo 	= {$mpevlrsaldo},
                                    mpevlrsub20h 	= {$mpevlrsub20h},
                                    mpevlrsub40h 	= {$mpevlrsub40h},
                                    mpeprocesso 	= {$mpeprocesso}
                                where mpeid = {$mpeid};
                        ";
//                        ver($sql, d);
                    }else{
                        $sql .= "
                            insert into academico.movprofequivalente(
                                    entid,ppeid,mpeano,mpemes,mpevlr20h,mpevlr40h,mpevlrdedexclusiva
                                    ,mpevlrsubstituto
                                    ,mpevlrvisitante,
                                    mpevlrvagos,mpevlrbcequiv,mpevlrsaldo,mpedtinclusao,mpestatus
                                    , mpevlrsub20h , mpevlrsub40h , mpeprocesso
                                )values(
                                    {$entid}, {$ppeid}, {$mpeano}, {$mpemes}, {$mpevlr20h}, {$mpevlr40h}, {$mpevlrdedexclusiva}
                                    , {$mpevlrsubstituto}
                                    ,{$mpevlrvisitante},{$mpevlrvagos},{$mpevlrbcequiv},{$mpevlrsaldo},now(),'A'
                                    , {$mpevlrsub20h} , {$mpevlrsub40h} , {$mpeprocesso}
                            );
                        ";
                    }
                }

                if(!empty($sql)){
                    $db->executar($sql);
                    if($db->commit()){
                        echo 1;
                        die;
                    }
                }
            }
	}
	echo 0;
	die;
    }

    include_once APPRAIZ . "includes/cabecalho.inc";
    echo "<br/>";

    if($_GET){
        extract($_GET);        
    }

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
    function float2moeda(num) {
        var x = 0;

        if( num <0 ){
           num = Math.abs(num);
           x = 1;
        }

        if( isNaN(num) ){
            num = "0";
        }
        cents = Math.floor((num*100+0.5)%100);

        num = Math.floor((num*100+0.5)/100).toString();

        if(cents < 10){ 
            cents = "0" + cents;
        }
        
        for (var i = 0; i < Math.floor((num.length-(1+i))/3); i++){
            num = num.substring(0,num.length-(4*i+3))+ '.' +num.substring(num.length-(4*i+3));
        }
        
        ret = num + ',' + cents;

        if (x == 1){ 
            ret = ' - ' + ret; 
        }
        return ret;
    }

    $(function(){
        $('[name=ano], [name=mes]').change(function(){

            ano = $('[name=ano]').val();
            mes = $('[name=mes]').val();

            if(ano != '' && mes != ''){
                $.ajax({
                    url : 'academico.php?modulo=principal/bancoProfEquivalente&acao=A',
                    type : 'post',
                    data : 'requisicao=carregaComboPortarias&ano='+ano+'&mes='+mes,
                    success : function(e){
                        $('#td_portaria').html(e);
                    }
                });
            }

            $('#divListaUniversidades').hide();
        });

        $('[name=portaria]').live('change',function(){
            ano = $('[name=ano]').val();
            mes = $('[name=mes]').val();
            
            semestre = mes <= 6 ? 1 : 2;

            document.location.href = 'academico.php?modulo=principal/bancoProfEquivalente&acao=A&ano='+ano+'&semestre='+semestre+'&mes='+mes+'&portaria='+this.value;
        });

        $('.calculoTotal').change(function(){
            entid = this.id.split('_')[1];

            mpevlr20h           = parseInt($('#mpevlr20h_'+entid).val().replace('.',''));
            mpevlr40h 		= parseInt($('#mpevlr40h_'+entid).val().replace('.',''));
            mpevlrdedexclusiva 	= parseInt($('#mpevlrdedexclusiva_'+entid).val().replace('.',''));

            ptvvalor 		= parseFloat(replaceAll($('#ptvvalor_'+entid).html(), '.', '').replace(',','.'));
            mpevlrsubstituto 	= parseFloat(replaceAll($('#mpevlrsubstituto_'+entid).val(), '.', '').replace(',','.'));
            mpevlrvisitante 	= parseFloat(replaceAll($('#mpevlrvisitante_'+entid).val(), '.', '').replace(',','.'));

            total = (parseInt(mpevlr20h)+parseInt(mpevlr40h)+parseInt(mpevlrdedexclusiva)+parseInt(mpevlrsubstituto)+parseInt(mpevlrvisitante)).toFixed(2);
            $('#total_'+entid).html(float2moeda(total));

            calculo = ((parseInt(mpevlr20h)*0.58)+(parseInt(mpevlr40h)*1)+(parseFloat(mpevlrdedexclusiva)*1.7)+(parseFloat(mpevlrsubstituto)*1)+(parseFloat(mpevlrvisitante)*1.7));
            $('#mpevlrbcequiv_'+entid).html(float2moeda(calculo));
            $('[name=mpevlrbcequiv['+entid+']]').val(float2moeda(calculo));

            saldo = (parseFloat(ptvvalor)-calculo).toFixed(2);
            $('#saldo_'+entid).html(float2moeda(saldo));
            $('[name=mpevlrsaldo['+entid+']]').val(float2moeda(saldo));
        });

        $('#btnVoltar').click(function(){
            document.location.href = 'academico.php?modulo=principal/bancoProfEquivalente&acao=A';
        });

        $('#btnSalvar').click(function(){
            params1 = $('[name=formulario]').serialize();
            params2 = $('[name=formValores]').serialize();

            $.ajax({
                url : 'academico.php?modulo=principal/bancoProfEquivalente&acao=A',
                type : 'post',
                data : 'requisicao=salvaDados&'+params1+'&'+params2,
                success	: function(e){
                    //console.log(e);
                    if(e == 1){
                        alert('Operação realizada com sucesso.');
                    }else{
                        alert('Não foi possível completar a operação!');
                    }
                }
            });
        });

        $('.selecionaLinha').focus(function(){
            $(this).parent().parent().css('background', '#8CA9B6');
        });

        $('.selecionaLinha').blur(function(){
            $(this).parent().parent().css('background', '');
        });

    });
</script>

<?PHP

    $linha1 = "Banco de Professor Equivalente";
    $linha2 = "Relatório";

    monta_titulo($linha1, $linha2);
?>

<form name="formulario" id="formulario" method="post" action="">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="subtituloDireita" width="160">Ano</td>
            <td>
                <?PHP
                    $arDados = array(
                        array('codigo' => '2008', 'descricao' => '2008'),
                        array('codigo' => '2009', 'descricao' => '2009'),
                        array('codigo' => '2010', 'descricao' => '2010'),
                        array('codigo' => '2011', 'descricao' => '2011'),
                        array('codigo' => '2012', 'descricao' => '2012'),
                        array('codigo' => '2013', 'descricao' => '2013'),
                        array('codigo' => '2014', 'descricao' => '2014'),
                        array('codigo' => '2015', 'descricao' => '2015'),
                        array('codigo' => '2016', 'descricao' => '2016'),
                        array('codigo' => '2017', 'descricao' => '2017'),
                        array('codigo' => '2018', 'descricao' => '2018'),
                        array('codigo' => '2019', 'descricao' => '2019'),
                        array('codigo' => '2020', 'descricao' => '2020')
                    );
                    $db->monta_combo('ano', $arDados, 'S', 'Selecione...', '', '', '', '', 'S');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Mês</td>
            <td id="td_mes">
                <?PHP
                    $arDados = array(
                        array('codigo' => '1', 'descricao' => 'Janeiro'),
                        array('codigo' => '2', 'descricao' => 'Fevereiro'),
                        array('codigo' => '3', 'descricao' => 'Março'),
                        array('codigo' => '4', 'descricao' => 'Abril'),
                        array('codigo' => '5', 'descricao' => 'Maio'),
                        array('codigo' => '6', 'descricao' => 'Junho'),
                        array('codigo' => '7', 'descricao' => 'Julho'),
                        array('codigo' => '8', 'descricao' => 'Agosto'),
                        array('codigo' => '9', 'descricao' => 'Setembro'),
                        array('codigo' => '10', 'descricao' => 'Outubro'),
                        array('codigo' => '11', 'descricao' => 'Novembro'),
                        array('codigo' => '12', 'descricao' => 'Dezembro')
                    );
                    $db->monta_combo('mes', $arDados, 'S', 'Selecione...', '', '', '', '', 'S');
                ?>
            </td>
        </tr>
        <tr>
            <td class="subtituloDireita">Portaria</td>
            <td id="td_portaria">
                <select name="portaria" class="CampoEstilo">
                    <option value="">Selecione...</option>
                </select>
            </td>
        </tr>
    </table>
</form>

<div id="divListaUniversidades">
    <?PHP
    
        if($_GET['ano']){
            if(verificaTipoPesquisa($_GET) == 'A'){
                $cabecalho = array('Sigla', 'Órgão', 'UF','Nº Portaria', 'Ano', 'Mês', '20h', '40h', 'DE', 'Subst.', 'Visit.', 'Vagos', 'Total', 'BancoPEqv(ocupado)', 'Portaria MEC BPEQ', 'Saldo');
                $sql = montaSqlProfEquivalente($_POST);
            } else {
                $cabecalho = array('Sigla', 'Órgão', 'UF','Nº Portaria', 'Ano', 'Mês', 'FF 20h', 'FF 40h', 'DE', 'Sub. 20h' , 'Sub. 40h', 'Visit.', 'Vagos', 'Total', 'BancoPEqv(ocupado)', 'Portaria MEC BPEQ', 'Saldo');
                $sql = montaSqlProfEquivalenteNovo($_POST);
            }

            $db->monta_lista($sql, $cabecalho, 1000000, 10, 'N', '', '', 'formValores', '');
        }
    ?>
    <center>
        <p>
            <input type="button" value="Salvar" id="btnSalvar" />
            <input type="button" value="Voltar" id="btnVoltar" />
        </p>
    </center>
</div>

<?php if($_GET['portaria']){ ?>
    <script>
        $(function(){
            $.ajax({
                url : 'academico.php?modulo=principal/bancoProfEquivalente&acao=A',
                type : 'post',
                data : 'requisicao=carregaComboPortarias&ano='+<?=$_GET['ano']?>+'&mes='+<?=$_GET['mes']?>,
                success	: function(e){
                    $('#td_portaria').html(e);
                    $('[name=portaria]').val(<?=$_GET['portaria']?>);
                }
            });
        });
    </script>
<?php } ?>
