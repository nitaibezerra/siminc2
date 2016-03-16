<?PHP
    $_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];


    if( $_REQUEST['requisicao'] == 'inserirDadosModAmb' ){
        inserirDadosModAmb( $_REQUEST );
    }

    if( $_REQUEST['ajax_campus_inserir'] ){
        $audid              = $_REQUEST['audid'];

        $entidunidade       = $_REQUEST['entunidade'];
        $entidcampus        = $_REQUEST['entcampus'];
        $ambid              = $_REQUEST['ambid'];
        $mdlid              = $_REQUEST['mdlid'];

        $audqtd             = $_REQUEST['audqtd'] != '' ? $_REQUEST['audqtd'] : '0000';
        $audmobiliada       = $_REQUEST['audmobiliada'] != '' ? "'".$_REQUEST['audmobiliada']."'" : 'NULL';
        $auddsc             = utf8_decode((string)addslashes($_REQUEST['auddsc']));
        $audareatotal       = $_REQUEST['audareatotal'] != '' ? $_REQUEST['audareatotal'] : '000';

        $audacessibilidade  = $_REQUEST['audacessibilidade'] != '' ? "'".$_REQUEST['audacessibilidade']."'" : 'NULL';
        $audnivelqldequip   = $_REQUEST['audnivelqldequip'] != '' ? "'".$_REQUEST['audnivelqldequip']."'" : 'NULL';
        $audnivelqtdequip   = $_REQUEST['audnivelqtdequip'] != '' ? "'".$_REQUEST['audnivelqtdequip']."'" : 'NULL';
        $audclimatizado     = $_REQUEST['audclimatizado'] != '' ? "'".$_REQUEST['audclimatizado']."'" : 'NULL';

        if( $entidunidade > 0 && $entidcampus > 0 && $ambid > 0 && $mdlid > 0 ){
            if( $audid > 0 ){
                $sql = "
                    UPDATE academico.unidadeambiente
                        SET entidunidade        = {$entidunidade},
                            entidcampus         = {$entidcampus},
                            ambid               = {$ambid},
                            mdlid               = {$mdlid},
                            audqtd              = '{$audqtd}',
                            audmobiliada        = {$audmobiliada},
                            auddsc              = '{$auddsc}',
                            audareatotal        = '{$audareatotal}',
                            audacessibilidade   = {$audacessibilidade},
                            audnivelqldequip    = {$audnivelqldequip},
                            audnivelqtdequip    = {$audnivelqtdequip},
                            audclimatizado      = {$audclimatizado}
                        WHERE audid = {$audid} RETURNING audid
                ";
            }else{
                $sql = "
                    INSERT INTO academico.unidadeambiente(
                        entidunidade, entidcampus, ambid, mdlid, audqtd, audmobiliada, auddsc, audareatotal, audacessibilidade, audnivelqldequip, audnivelqtdequip, audclimatizado
                    ) VALUES (
                        {$entidunidade},{$entidcampus},{$ambid},{$mdlid},'{$audqtd}',{$audmobiliada},'{$auddsc}','{$audareatotal}',{$audacessibilidade},{$audnivelqldequip},{$audnivelqtdequip},{$audclimatizado}
                    ) RETURNING audid
                ";
            }
            $audid = $db->pegaUm($sql);
        }

        if( $audid > 0 ){
            $db->commit();
            
            $ambdsc = $db->pegaUm("SELECT ambdsc FROM academico.ambiente where ambid = {$ambid}");
            
            echo '<resp>';
?>
            <td class="SubTituloCentro"> &nbsp; </td>
            <td class="SubTituloCentro">
                <a class="various fancybox.ajax" href="/academico/academico.php?modulo=principal/listainfrafisica&acao=A&mdlid=<?=$mdlid;?>&ambid=<?=$ambid;?>&audid=<?=$audid;?>&entidunidade=<?=$entidunidade;?>&entidcampus=<?=$entidcampus;?>">
                    <img src="/imagens/alterar.gif" border="0" title="Editar" style="cursor:pointer;">
                </a>
            </td>
            <td class="SubTituloDireita"> <?=$ambdsc;?> </td>
            <td class="SubTituloCentro">
                <?PHP
                    if($_REQUEST['audacessibilidade'] == 'C'){
                        echo "Completa";
                    }
                    if($_REQUEST['audacessibilidade'] == 'P'){
                        echo "Parcial";
                    }
                    if($_REQUEST['audacessibilidade'] == 'I'){
                        echo "Inexistente";
                    }
                ?>
            </td>
            <td class="SubTituloCentro">
                <?PHP
                    if ($_REQUEST['audnivelqldequip'] == 'O'){
                        echo "Ótima";
                    }
                    if($_REQUEST['audnivelqldequip'] == 'B'){
                        echo "Bom";
                    }
                    if($_REQUEST['audnivelqldequip'] == 'R'){
                        echo "Regular";
                    }
                    if($_REQUEST['audnivelqldequip'] == 'N'){
                        echo "Não se Aplica";
                    }
                ?>
            </td>
            <td class="SubTituloCentro">
                <?PHP
                    if ($_REQUEST['audnivelqtdequip'] == 'C'){
                        echo "Completa";
                    }
                    if ($_REQUEST['audnivelqtdequip'] == 'P'){
                        echo "Parcial";
                    }
                    if ($_REQUEST['audnivelqtdequip'] == 'I'){
                        echo "Inexistente";
                    }
                ?>
            </td>
            <td class="SubTituloCentro"> <?=$unddsc ? $unddsc : 'Unidade' ;?> </td>
            <td class="SubTituloCentro"> <?=$audqtd;?> </td>
            <td class="SubTituloCentro"> <?=$audareatotal;?> </td>
            <td class="SubTituloCentro">
                <?PHP
                    if($_REQUEST['audmobiliada'] == 't'){
                        echo "Sim";
                    }
                    if($_REQUEST['audmobiliada'] == 'f'){
                        echo "Não";
                    }
                ?>
            </td>
            <td class="SubTituloCentro">
                <?PHP
                    if($_REQUEST['audclimatizado'] == 't'){
                        echo "Sim";
                    }
                    if($_REQUEST['audclimatizado'] == 'f'){
                        echo "Não";
                    }
                ?>
            </td>
            <td class="SubTituloJustificado"> <?=$auddsc;?> </td>
    <?php
            echo '</resp>';            
        }
        die();
    }

    if( $_REQUEST['mdlid'] && !$_REQUEST['ajax_campus_inserir'] ){
        $audid = $_REQUEST['audid'];
        $ambid = $_REQUEST['ambid'];

        if (!empty($audid)) {
            $entidunidade = $_REQUEST['entidunidade'];
            $entidcampus = $_REQUEST['entidcampus'];

            $sql = "select * from academico.unidadeambiente where audid = $audid";
            $dados = $db->pegalinha($sql);
        }

        if (!empty($ambid)) {
            $sql = "
                SELECT u.unddsc
                FROM academico.ambiente a
                LEFT JOIN academico.unidademed u ON u.undid = a.undid
                WHERE a.ambid = {$ambid}
            ";
            $unddsc = $db->pegaUm($sql);
        }
?>
        <input type="hidden" name="audid" value="<?=$audid;?>">
        <input type="hidden" name="entunidade" value="<?=$_REQUEST['entidunidade'];?>">
        <input type="hidden" name="entcampus" value="<?=$_REQUEST['entidcampus'];?>">
        <input type="hidden" name="mdlid" value="<?=$_REQUEST['mdlid'];?>">
        <input type="hidden" name="ambid" value="<?=$_REQUEST['ambid'];?>">
        <table style="width: 800px">
            <tr>
                <td class="SubTituloEsquerda" width="25%"> Mobiliada: </td>
                <td class="CampoEsquerda">
                    <input type="radio" name="mobiliada" value="t" <?PHP if ($dados['audmobiliada'] == 't') echo "checked='checked'"; ?>> Sim
                    <input type="radio" name="mobiliada" value="f" style="margin-left:33px;" <?PHP if ($dados['audmobiliada'] == 'f') echo "checked='checked'"; ?>> Não
                </td>
            </tr>
            <tr>
                <td class="SubTituloEsquerda"> Climatizado: </td>
                <td class="CampoEsquerda">
                    <input type="radio" name="climatizado" value="t" <?PHP if ($dados['audclimatizado'] == 't') echo "checked='checked'"; ?>> Sim
                    <input type="radio" name="climatizado" value="f" style="margin-left:33px;" <?PHP if ($dados['audclimatizado'] == 'f') echo "checked='checked'"; ?>> Não
                </td>
            </tr>
            <tr>
                <td class="SubTituloEsquerda">Acessibilidade: </td>
                <td class="CampoEsquerda">
                    <input type="radio" name="acessibilidade" value="C" <?PHP if ($dados['audacessibilidade'] == 'C') echo "checked='checked'"; ?>> Completa
                    <input type="radio" name="acessibilidade" value="P" style="margin-left:5px;" <?PHP if ($dados['audacessibilidade'] == 'P') echo "checked='checked'"; ?>> Parcial
                    <input type="radio" name="acessibilidade" value="I" style="margin-left:6px;" <?PHP if ($dados['audacessibilidade'] == 'I') echo "checked='checked'"; ?>> Inexistente
                </td>
            </tr>
            <tr>
                <td class="SubTituloEsquerda"> Nível Qualidade Equipamento(s): </td>
                <td class="CampoEsquerda">
                    <input type="radio" name="nivelqldequip" value="O" <?PHP if ($dados['audnivelqldequip'] == 'O') echo "checked='checked'"; ?>> Ótima
                    <input type="radio" name="nivelqldequip" value="B"  style="margin-left:23px;" <?PHP if ($dados['audnivelqldequip'] == 'B') echo "checked='checked'"; ?>> Bom
                    <input type="radio" name="nivelqldequip" value="R" style="margin-left:16px;" <?PHP if ($dados['audnivelqldequip'] == 'R') echo "checked='checked'"; ?>> Regular
                    <input type="radio" name="nivelqldequip" value="N" style="margin-left:8px;" <?PHP if ($dados['audnivelqldequip'] == 'N') echo "checked='checked'"; ?>> Não se Aplica
                </td>
            </tr>
            <tr>
                <td class="SubTituloEsquerda"> Nível Quantidade Equipamento(s): </td>
                <td class="CampoEsquerda">
                    <input type="radio" name="nivelqtdequip" value="C" <?PHP if ($dados['audnivelqtdequip'] == 'C') echo "checked='checked'"; ?>> Completa
                    <input type="radio" name="nivelqtdequip" value="P" style="margin-left:6px;" <?PHP if ($dados['audnivelqtdequip'] == 'P') echo "checked='checked'"; ?>> Parcial
                    <input type="radio" name="nivelqtdequip" value="I" style="margin-left:5px;" <?PHP if ($dados['audnivelqtdequip'] == 'I') echo "checked='checked'"; ?>> Inexistente
                </td>
            </tr>
            <tr>
                <td class="SubTituloEsquerda"> Unidade: </td>
                <td class="CampoEsquerda">
                    <?=$unddsc?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloEsquerda"> Quantidade: </td>
                <td class="CampoEsquerda">
                    <input type="text" onKeyUp= "this.value=mascaraglobal('########.##',this.value);" name="qtd" style="width: 450px" value="<?=$dados['audqtd'];?>">
                </td>
            </tr>
            <tr>
                <td class="SubTituloEsquerda"> Área Total (m²): </td>
                <td class="CampoEsquerda">
                    <input type="text" onKeyUp= "this.value=mascaraglobal('######.##',this.value);"; name="audareatotal" style="width: 450px" value="<?=$dados['audareatotal'];?>">
                </td>
            </tr>
            <tr>
                <td class="SubTituloEsquerda"> Descrição: </td>
                <td class="CampoEsquerda">
                    <textarea name="dsc" style="width: 450px;height: 80px" maxlength="500"><?PHP echo $dados['auddsc'] ?></textarea>
                </td>
            </tr>
            <tr>
                <td colspan="2" style="text-align: right">
                    <input type="button" value="Salvar" style="margin-top: 10px;" class="bt_enviar_insert">
                </td>
            </tr>
        </table>

        <script>
            $1_11(".bt_enviar_insert").click(function () {
                var mobiliada       = $1_11("input[name=mobiliada]:checked").val();
                var climatizado     = $1_11("input[name=climatizado]:checked").val();
                var acessibilidade  = $1_11("input[name=acessibilidade]:checked").val();
                var nivelqldequip   = $1_11("input[name=nivelqldequip]:checked").val();
                var nivelqtdequip   = $1_11("input[name=nivelqtdequip]:checked").val();
                var qtd             = $1_11("input[name=qtd]").val();
                var audareatotal    = $1_11("input[name=audareatotal]").val();
                var dsc             = $1_11("textarea[name=dsc]").val();
                var mdlid           = $1_11("input[name=mdlid]").val();
                var ambid           = $1_11("input[name=ambid]").val();
                var entunidade      = $1_11("input[name=entunidade]").val();
                var entcampus       = $1_11("input[name=entcampus]").val();
                var audid           = $1_11("input[name=audid]").val();

                $1_11.post("?modulo=principal/listainfrafisica&acao=A", {
                    ajax_campus_inserir: "1",
                    audmobiliada: mobiliada,
                    audclimatizado: climatizado,
                    audacessibilidade: acessibilidade,
                    audnivelqldequip: nivelqldequip,
                    audnivelqtdequip: nivelqtdequip,
                    audqtd: qtd,
                    audareatotal: audareatotal,
                    auddsc: dsc,
                    mdlid: mdlid,
                    ambid: ambid,
                    entunidade: entunidade,
                    entcampus: entcampus,
                    audid: audid
                }, function(data){
                    var resp = pegaRetornoAjax('<resp>', '</resp>', data, true);
                    $1_11('#tr_md_'+mdlid+'_amb_'+ambid).html( resp );
                    $1_11('#tr_md_'+mdlid+'_amb_'+ambid+' td').css('color', 'green');
                    window.parent.$1_19.fancybox.close();
                    setTimeout(function(){ $1_11('#tr_md_'+mdlid+'_amb_'+ambid+' td').css('color', ''); }, 3000);
                });
            });
        </script>
    <?PHP
        die();
    }
    
    if($_REQUEST['ajax_campus']){
        $entidcampus = addslashes($_REQUEST['entidcampus']);

        //veririca entidade
        $sql = "
            SELECT ef.funid
            FROM entidade.funcaoentidade ef
            JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
            WHERE ef.entid = {$entidcampus}
        ";
        $funid = $db->pegaUm($sql);

        $sql = "
            SELECT  audid, auddsc,
                    ua.entidcampus, ua.entidunidade, ua.entidcampus, ua.entidcampus as entcampus,
                    mdldsc, m.mdlid,
                    a.ambid, ambdsc, audqtd, audareatotal, audmobiliada, audclimatizado,
                    audacessibilidade, audnivelqldequip, audnivelqtdequip, um.unddsc
            FROM academico.modulo m
            LEFT JOIN academico.moduloambiente ma on m.mdlid = ma.mdlid
            LEFT JOIN academico.ambiente a on a.ambid = ma.ambid
            LEFT JOIN academico.unidademed um on um.undid = a.undid
            LEFT JOIN academico.unidadeambiente ua on ua.ambid = a.ambid AND ma.mdlid = ua.mdlid AND ua.entidcampus = {$entidcampus}

            ORDER BY mdldsc, ambdsc
        ";
        $modulos = $db->carregar($sql);
    ?> 
        <div style="width:100%; margin-top:5px; margin-bottom:20px;">
            <table align="center" border="1" cellspacing="0" cellpadding="2" style="color:#333333;" class="tabela" width="100%">
                <thead>
                    <tr>
                        <td colspan="13" class="SubTituloCentro"> <h5> <b>Listas de módulos</b> </h5> </td>
                    </tr>
                </thead>
                <?PHP
                    $mdlid = 0;
                    foreach( $modulos as $i => $key ){
                        if ($mdlid != $key['mdlid']) {
                ?>
                            <thead>
                                <tr>
                                    <td style="width:3%; text-align:center;">
                                        <img class="bt_mais_arvore" id="bt_mm_ent_<?=$entidcampus?>_md_<?=$key['mdlid']?>" name="ent_<?=$entidcampus?>_md_<?=$key['mdlid']?>" onclick="abrirArvore(this)" src="../imagens/mais.gif" style="padding-right:5px; cursor:pointer;" width="9" height="9"/>
                                    </td>
                                    <td colspan="12" style="font-size:11px; font-weight:bolder;"> Módulo: <?PHP echo $key['mdldsc'] ?> </td>
                                </tr>
                                <tr id="tr_head_ent_<?=$entidcampus?>_md_<?=$key['mdlid']?>" style="display:none;">
                                    <td class="SubTituloCentro"> &nbsp; </td>
                                    <td class="SubTituloCentro" width="5%"> Ação </td>
                                    <td class="SubTituloCentro" width="20%"> Ambiente </td>
                                    <td class="SubTituloCentro" width="10%"> Acessibilidade </td>
                                    <td class="SubTituloCentro" width="10%"> Nível Qualidade Equipamento(s) </td>
                                    <td class="SubTituloCentro" width="10%"> Nível Quantidade Equipamento(s) </td>
                                    <td class="SubTituloCentro" width="10%"> Unidade </td>
                                    <td class="SubTituloCentro" width="10%"> Quantidade </td>
                                    <td class="SubTituloCentro" width="10%"> Área Total (m²) </td>
                                    <td class="SubTituloCentro" width="10%"> Mobiliada </td>
                                    <td class="SubTituloCentro" width="10%"> Climatizado </td>
                                    <td class="SubTituloCentro"> Descrição </td>
                                </tr>
                            </thead>
                            <tbody id="tb_ddmd_ent_<?=$entidcampus?>_md_<?=$key['mdlid']?>" style="background-color:white !important; display:none;">
                <?PHP   } ?>
                                <tr id="tr_md_<?=$key['mdlid']?>_amb_<?=$key['ambid']?>">
                                    <td class="SubTituloCentro"> &nbsp; </td>
                                    <td class="SubTituloCentro">
                                        <a class="various fancybox.ajax" href="/academico/academico.php?modulo=principal/listainfrafisica&acao=A&mdlid=<?=$key['mdlid']; ?>&ambid=<?=$key['ambid']; ?>&audid=<?=$key['audid']; ?>&entidunidade=<?=$_REQUEST['entidunidade'] ?>&entidcampus=<?=$_REQUEST['entidcampus'] ?>">
                                            <img src="/imagens/alterar.gif" border="0" title="Editar" style="cursor:pointer;">
                                        </a>
                                        <!--<img src="/imagens/alterar.gif" border="0" title="Editar" style="cursor:pointer;" onclick="inserirDadosModAmb('<?=$key['audid'];?>', '<?=$key['ambid'];?>', '<?=$key['mdlid'];?>', '<?=$_REQUEST['entidunidade'];?>', '<?=$_REQUEST['entidcampus'];?>');">-->
                                    </td>
                                    <td class="SubTituloDireita"> <?=$key['ambdsc']; ?> </td>
                                    <td class="SubTituloCentro">
                                        <?PHP
                                        if ($key['audacessibilidade'] == 'C'){
                                            echo "Completa";
                                        }
                                        if ($key['audacessibilidade'] == 'P'){
                                            echo "Parcial";
                                        }
                                        if ($key['audacessibilidade'] == 'I'){
                                            echo "Inexistente";
                                        }
                                        ?>
                                    </td>
                                    <td class="SubTituloCentro">
                                        <?PHP
                                        if ($key['audnivelqldequip'] == 'O'){
                                            echo "Ótima";
                                        }
                                        if ($key['audnivelqldequip'] == 'B'){
                                            echo "Bom";
                                        }
                                        if ($key['audnivelqldequip'] == 'R'){
                                            echo "Regular";
                                        }
                                        if ($key['audnivelqldequip'] == 'N'){
                                            echo "Não se Aplica";
                                        }
                                        ?>
                                    </td>
                                    <td class="SubTituloCentro">
                                        <?PHP
                                        if ($key['audnivelqtdequip'] == 'C'){
                                            echo "Completa";
                                        }
                                        if ($key['audnivelqtdequip'] == 'P'){
                                            echo "Parcial";
                                        }
                                        if ($key['audnivelqtdequip'] == 'I'){
                                            echo "Inexistente";
                                        }
                                        ?>
                                    </td>
                                    <td class="SubTituloCentro"> <?=$key['unddsc']; ?> </td>
                                    <td class="SubTituloCentro"> <?=$key['audqtd']; ?> </td>
                                    <td class="SubTituloCentro"> <?=$key['audareatotal']; ?> </td>
                                    <td class="SubTituloCentro">
                                        <?PHP
                                            if ($key['audmobiliada'] == 't'){
                                                echo "Sim";
                                            }
                                            if ($key['audmobiliada'] == 'f'){
                                                echo "Não";
                                            }
                                        ?>
                                    </td>
                                    <td class="SubTituloCentro">
                                        <?PHP
                                            if ($key['audclimatizado'] == 't'){
                                                echo "Sim";
                                            }
                                            if ($key['audclimatizado'] == 'f'){
                                                echo "Não";
                                            }
                                        ?>
                                    </td>
                                    <td class="SubTituloJustificado"> <?=$key['auddsc']; ?> </td>
                                </tr>
    <?PHP
                        $mdlid = $key['mdlid'];
                        if ($mdlid != $key['mdlid']) {
    ?>
                            </tbody>
    <?PHP
                        }
                    }
    ?>
            </table>

     <?if($funid == ACA_ID_REITORIA){?>
            <table align="center" border="1" cellspacing="0" cellpadding="2" style="color:#333333;" class="tabela" width="100%">
                <tr>
                    <td class="SubTituloDireita">
                        <a href="?modulo=principal/listainfrafisicaexcel&acao=A&entidreitoria=<?PHP echo $entidcampus ?>">
                            <input type="button" value="Gerar excel">
                        </a>
                    </td>
                    <td class="SubTituloEsquerda">
                        <a href="?modulo=principal/listainfrafisicapdf&acao=A&entidreitoria=<?PHP echo $entidcampus ?>">
                            <input type="button" value="Gerar PDF">
                        </a>
                    </td>
                </tr>
            </table>
    <?}else{?>
         <table align="center" border="1" cellspacing="0" cellpadding="2" style="color:#333333;" class="tabela" width="100%">
             <tr>
                 <td class="SubTituloDireita">
                     <a href="?modulo=principal/listainfrafisicaexcel&acao=A&entidcampus=<?PHP echo $entidcampus ?>">
                         <input type="button" value="Gerar excel">
                     </a>
                 </td>
                 <td class="SubTituloEsquerda">
                     <a href="?modulo=principal/listainfrafisicapdf&acao=A&entidcampus=<?PHP echo $entidcampus ?>">
                         <input type="button" value="Gerar PDF">
                     </a>
                 </td>
             </tr>
         </table>
    <?}?>

        </div>

    <?PHP
        die();
    }

    //instancia o objeto do módulo
    $academico = new academico();

    //monta o cabeçalho
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br/>";

    // Monta as abas
    if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
        $abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57276' : '57135'; //Dados Desenv
    } else {
        $abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57267' : '57135';
    }
    #$db->cria_aba($abacod_tela,$url,$parametros);
    monta_titulo("Lista de Campus e Reitoria - Infraestrutura Física", "Rede Federal");

    $entid = $_SESSION['academico']['entid'];
    $orgid = $_SESSION['academico']['orgid'];

    #BUSCA PERFIL DO USUÁRIO
    $perfil = pegaPerfilGeral();

    /*
     * VALIDANDO SE EXISTE O ENTID
     * FEITO POR ALEXANDRE DOURADO 16/11/2009
     */
    if (!$entid || !$orgid) {
        echo "
            <script>
                alert('Problemas nas variáveis globais. Acesse novamente');
                window.location='academico.php?modulo=inicio&acao=C';
            </script>
        ";
        exit;
    }

    $bloqueado = academico_mensagem_bloqueio( $_SESSION['academico']['orgid'] );

    //QUERY CAMPUS
    switch ($orgid){
        case '1':
            $funid = ACA_ID_CAMPUS;
        break;
        case '2':
            $funid = ACA_ID_UNED;
        break;
    }

    if( in_array(PERFIL_DIRETOR_CAMPUS, $perfil) || in_array(PERFIL_INTERLOCUTOR_CAMPUS, $perfil) ){
        $sql = "SELECT entid FROM academico.usuarioresponsabilidade WHERE usucpf = '{$_SESSION['usucpf']}' AND pflcod IN (".PERFIL_DIRETOR_CAMPUS.",".PERFIL_INTERLOCUTOR_CAMPUS.")";
        $result = $db->carregarColuna($sql);
        $WHERE = "WHERE e.entid IN (" . implode($result, ',') . ")";
    }else{
        $WHERE = "WHERE ea.entid = {$entid} AND e.entstatus = 'A' AND ef.funid = {$funid}";
    }

    $_ACAO = "
        <img class=\"bt_mais\" src=\"../imagens/mais.gif\" style=\"cursor:pointer;\" width=\"9\" height=\"9\" id=\"'||e.entid||'\" name=\"img_'||e.entid||'\" />
    ";

    $_tr = "
        <tr>
            <td style=\"padding:0px;margin:0;\"></td>
            <td id=\"td' ||  e.entid || '\" colspan=\"6\"> </td>
            <td style=\"padding:0px;margin:0;\"> </td>
        </tr>
    ";

    $sql = "
        SELECT '{$_ACAO}' as acao,
                upper(e.entnome) as nome,
                upper(mun.mundescricao) as municipio,
                upper(mun.estuf) as uf,
                '{$_tr}' as tr
        FROM entidade.entidade e2

        INNER JOIN entidade.entidade e ON e2.entid = e.entid
        INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
        INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
        LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
        LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod

        {$WHERE}

        ORDER BY e.entnome
    ";

    //QUERY REITORIA
    if( $orgid ){
        $funidrt = ACA_ID_REITORIA;
    }else{
        $funidrt = ACA_ID_UNED;
    }


    $_ACAOrt = "
        <img class=\"bt_mais\" src=\"../imagens/mais.gif\" style=\"cursor:pointer;\" width=\"9\" height=\"9\" id=\"'||e.entid||'\" name=\"img_'||e.entid||'\" />
    ";

    $_trrt = "
        <tr>
            <td style=\"padding:0px;margin:0;\"></td>
            <td id=\"td' ||  e.entid || '\"> </td>
            <td style=\"padding:0px;margin:0;\"> </td>
        </tr>
    ";

    $sqlrt = "
        SELECT '{$_ACAOrt}' as acao,
                upper(e.entnome) as nome,
                '{$_trrt}' as tr

        FROM entidade.entidade e2

        INNER JOIN entidade.entidade e ON e2.entid = e.entid
        INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
        INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid

        WHERE ea.entid = {$entid} AND e.entstatus = 'A' AND ef.funid = {$funidrt}

        ORDER BY e.entnome
    ";
?>

<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros); ?>
</div>

<div class='col-md-9' style='position:static;'>
    <?PHP
        $cabecalho = array('Ação', 'Campus', 'Município', 'UF', '');
        $tamanho = Array('5%' , '' , '' , '');
        $alinhamento = Array('center', '', '', '');
        $db->monta_lista($sql, $cabecalho, 100, 30, 'N', 'center', 'N', '', $tamanho, $alinhamento);
    ?>
    <br>
    <?PHP

    if( in_array(PERFIL_INTERLOCUTOR_INSTITUTO, $perfil) || in_array(PERFIL_PROREITOR, $perfil) || in_array(PERFIL_REITOR, $perfil) ){
        $cabecalho = array('Ação', 'Reitoria', '');
        $tamanho = Array('5%' , '', '1%');
        $alinhamento = Array('center', '', '');
        $db->monta_lista($sqlrt, $cabecalho, 100, 30, 'N', 'center', 'N', '', $tamanho, $alinhamento);
    }
    ?>
</div>

<table align="center" border="0" cellspacing="0" cellpadding="2" style="color:#333333;" class="listagem">
    <tr>
        <td class="SubTituloDireita">
            <a href="?modulo=principal/listainfrafisicaexcel&acao=A">
                <input type="button" value="Gerar Excel">
            </a>
        </td>
        <td class="SubTituloEsquerda">
            <a href="?modulo=principal/listainfrafisicapdf&acao=A">
                <input type="button" value="Gerar PDF">
            </a>
        </td>
</table>

<script>
    function abrirArvore( obj ){
        var id      = obj.id;
        var classe  = obj.className;
        var modulo  = obj.name;

        if( classe == 'bt_mais_arvore' ){
            $1_11('#'+id).attr('src','../imagens/menos.gif').removeClass("bt_mais_arvore").addClass("bt_menos_arvore");
            $1_11('#tr_head_'+modulo).show();
            $1_11('#tb_ddmd_'+modulo).show();
        }else{
            $1_11('#'+id).attr('src','../imagens/mais.gif').removeClass("bt_menos_arvore").addClass("bt_mais_arvore");
            $1_11('#tr_head_'+modulo).fadeOut();
            $1_11('#tb_ddmd_'+modulo).fadeOut();
        }
    }

//    function inserirDadosModAmb(audid, ambid, mdlid, entidunidade, entidcampus){
//        $1_11.ajax({
//            type    : "POST",
//            url     : window.location,
//            data    : "requisicao=inserirDadosModAmb&audid="+audid+"&ambid="+ambid+"&mdlid="+mdlid+"&entidunidade="+entidunidade+"&entidcampus="+entidcampus,
//            async: false,
//            success: function(resp){
////                alert(resp);
////                $('#unidadeCadastro').html(resp);
//            }
//        });
//    }

    $1_11(document).ready(function () {
        $1_11('.bt_mais').click( function(){
            var id      = $1_11(this).attr('id');
            var classe  = $1_11(this).prop('class');

            if( classe == 'bt_mais' ){
                $1_11(this).attr('src','../imagens/menos.gif').removeClass("bt_mais").addClass("bt_menos");

                $1_11.post("?modulo=principal/listainfrafisica&acao=A", {
                    ajax_campus: "1",
                    entidcampus: id,
                    entidunidade:<?PHP echo $_SESSION['academico']['entid']?>
                }, function (data) {
                    $1_11("#td" + id).html(data);
                    $1_11("#td" + id).show();
                });
            }else{
                $1_11(this).attr('src','../imagens/mais.gif').removeClass("bt_menos").addClass("bt_mais");
                $1_11("#td" + id).fadeOut();
            }
        });
    })

    function abredadoscampus(entid) {
        return window.location = '?modulo=principal/alterarCampus&acao=A&entid=' + entid;
    }
</script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
<script type="text/javascript">
    $1_19 = $.noConflict();
</script>

<link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen"/>
<script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>

<script>
    $1_19(document).ready(function () {
        $1_19(".various").fancybox({
            type: "ajax",
            afterClose: function () {
                $1_19(".atualizar").trigger('click');
            }
        });
    });
</script>