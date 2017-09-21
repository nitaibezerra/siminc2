<?php
 /**
 * Classe auxiliar
 * @author LindalbertoFilho
 *
 */
class FuncoesBootstrap{
	
	/**
     * Função que carrega os orgãos de acordo com o tipo
     * e com o município informado.
     *	
     * @Obs.: Copia efetuada em cima da função carrega_orgao efetuada por Fernando A. Bagno da Silva
     *
     * @author Lindalberto Filho
     * @param string $editavel -> se os campos estao habilitados ou não
     * @param string $usucpf -> cpf do usuário cadastrado, caso exista
     * /
     */
    function carrega_orgao_bootstrap($editavel = "S", $usucpf){	
        global $muncod, $regcod, $tpocod, $entid, $db;

        if ($editavel == 'N'){
            // Se o usuário já está cadastrado no SIMEC, busca o orgão
            $sql = "
                SELECT
                    u.entid as codigo,
                    CASE 
                        WHEN ee.entorgcod IS NOT NULL
                            THEN ee.entorgcod ||' - '|| ee.entnome
                        ELSE ee.entnome 
                    END AS descricao
                FROM seguranca.usuario u
                INNER JOIN entidade.entidade ee ON ee.entid = u.entid
                WHERE u.usucpf = '{$usucpf}' 
                    AND ee.entorgcod <> '73000'
            ";

            inputCombo("entid", $sql, $valor, "entid",array('acao'=>'carregarUnidade','habil'=>$editavel));
        }else {
            // Se for Estadual ou Outros, o usuário deverá informar o orgão
            if ( $tpocod == 4){
                inputTexto("orgao", $valor, 'orgao', 100,false);
            }else{	
                // Caso o tipo seja municipal, verifica o municipio escolhido
                $inner = ( $tpocod == 3 || $tpocod == 2 ) ? 'INNER JOIN	entidade.endereco eed ON eed.entid = ee.entid': '';
                $uniao = ( $tpocod == 3 || $tpocod == 2 ) ?  "UNION ALL (SELECT 999999 AS codigo,'OUTROS' AS descricao)" : '';
                if ($tpocod == 2){
                    $regcod = ($_REQUEST['regcod']) ? $_REQUEST['regcod'] : $regcod;
                    $clausula = "AND eed.estuf = '" . $regcod . "'";
                }else if ($tpocod == 3){
                    $clausula = "AND eed.muncod = '" . $muncod . "'";
                }                

                $sql = "(SELECT
                            ee.entid AS codigo,
                            CASE 
                                WHEN ee.entorgcod IS NOT NULL 
                                    THEN ee.entorgcod ||' - '|| ee.entnome
                                ELSE ee.entnome 
                                END AS descricao
                        FROM entidade.entidade ee
                        INNER JOIN entidade.funcaoentidade ef ON ef.entid = ee.entid
                        INNER JOIN public.tipoorgaofuncao tpf ON ef.funid = tpf.funid
                        " . $inner . "
                        WHERE
                            ee.entstatus = 'A' AND
                            tpf.tpocod = '{$tpocod}'
                            " . $clausula . " AND
                            (ee.entorgcod IS NULL OR ee.entorgcod <> '73000' )
                        ORDER BY
                            ee.entnome)" . $uniao;
                            
                inputCombo("entid", $sql, $valor, "entid",array('acao' => 'carregarUnidade'));
            }	
        }	
    }
    
    /**
     * Função que carrega as unidades daquele órgão (caso
     * existam)
     *
     * @Obs.: Copia efetuada em cima da função carrega_orgao efetuada por Fernando A. Bagno da Silva

     * @author Lindalberto Filho
     * @param integer $entid -> ID do orgão selecionado
     * @param string $editavel -> se os campos estao habilitados ou não
     * @param string $usucpf -> cpf do usuário cadastrado, caso exista
     */
    function carrega_unidade_bootstrap($entid, $editavel, $usucpf){
        global $db;

        if ($editavel == null){
            $editavel = 'S';
        }
        if ( $entid == 999999 ){
            echo "<p class=\"form-control-static\">";
            inputTexto("orgao", $valor, "orgao", 50,false,array('habil'=>$editavel));
            echo "</p>";
        } else {	
            if ($editavel == 'N' ){	
                $sql = "SELECT
                            u.unicod AS codigo,
                            u.unicod||' - '||u.unidsc AS descricao
                        FROM unidade u
                        INNER JOIN seguranca.usuario su ON u.unicod = su.unicod
                        WHERE usucpf = '{$usucpf}'";
                inputCombo("unicod", $sql, $valor, "unicod",array('acao' => 'selecionar_unidade_orcamentaria','habil'=>'N'));
            }else {	
               if (!empty($entid) ){	
                   $uo_total = $db->pegaUm(" SELECT
                                                count(u.unicod)
                                             FROM unidade u
                                             INNER JOIN entidade.entidade e ON u.orgcod = e.entorgcod
                                             WHERE unistatus='A' 
                                             AND unitpocod='U' 
                                             AND e.entid ='{$entid}'");
                }
                if ($uo_total > 0){	
                    $sql = "
                        SELECT DISTINCT
                            u.unicod AS codigo,
                            u.unicod||' - '||unidsc AS descricao
                        FROM unidade u
                        INNER JOIN entidade.entidade e ON orgcod = entorgcod
                        WHERE unistatus='A' 
                            AND u.unitpocod='U' 
                            AND entid = '{$entid}'
                        ORDER BY u.unicod";      
                    inputCombo("unicod", $sql, $valor, "unicod",array('acao' => 'carregarUnidadeGestora'));
                } else {
                    if(!$entid || trim($entid) == "")
                        echo '<p id="unicod" class="form-control-static" style="color:#428bca;">Selecione um Órgão.</p>';
                    else                            
                        echo '<p id="unicod" class="form-control-static" style="color:#f0ad4e;">Este órgão não possui uma Unidade Orçamentária.</p>';
                }
            }
        }
    }
    
    /**
     * Função que carrega a unidade gestora de uma unidade orçamentária,
     * quando existir
     * @Obs.: Copia efetuada em cima da função carrega_orgao efetuada por Fernando A. Bagno da Silva

     * @author Lindalberto Filho
     * @param integer $unicod -> ID da unidade selecionada
     * @param string $editavel -> se os campos estao habilitados ou não
     * @param string $usucpf -> cpf do usuário cadastrado, caso exista
     */
    function carrega_unidade_gestora_bootstrap($unicod, $editavel, $usucpf){

        global $db;
        if ($editavel == null){
            $editavel = 'S';
        }

        if ( $editavel == 'N' ){
            $sql = "
                SELECT
                    ug.suocod AS codigo,
                    ug.suocod || ' - ' || ug.suonome as descricao
                FROM public.vw_subunidadeorcamentaria ug
                    JOIN seguranca.usuario su ON ug.suocod = su.ungcod
                WHERE
                    ug.suostatus = 'A' 
                    AND ug.prsano = '". (int)$_SESSION['exercicio']. "'
                    AND su.unicod = '".$unicod."' 
                    AND su.usucpf = '".$usucpf."'
                ORDER BY
                    suonome";
//ver($sql,d);
            inputCombo("ungcod", $sql, $valor, "ungcod",array('habil'=>$editavel));
        }else {	
            /*
             * FOI REMOVIDO PORQUE É NECESSÁRIO CADASTRAR USUÁRIOS DE UOs QUE NÃO SÃO DO MEC
            * ESCOLHENDO A UNIDADE GESTORA, PROBLEMA OCORRIDO NA PROGRAMAÇÃO ORÇAMENTÁRIA
            * TERMO DE COOPERAÇÃO, REQUISITANTE GUILHERME BORGES
            * DATA: 10/10/2013
            */

            $sql = "
                SELECT
                    suocod AS codigo,
                    suocod || ' - ' || suonome as descricao
                FROM public.vw_subunidadeorcamentaria
                WHERE
                    suostatus = 'A'
                    AND prsano = '". (int)$_SESSION['exercicio']. "'
                    AND unocod = '".$unicod."'
                ORDER BY
                    suonome";
            
            if($db->carregar($sql)){
                inputCombo("ungcod", $sql, $valor, "ungcod");
            }else{
                if(!$unicod || trim($unicod) == "")
                    echo '<p id="ungcod" class="form-control-static" style="color:#428bca;">Selecione uma Unidade Orçamentária.</p>';
                else
                    echo '<p id="ungcod" class="form-control-static" style="color:#f0ad4e;">Esta Unidade Orçamentária não possui uma Sub-Unidade.</p>';
            }            
        }
    }
    
        /**
     * --------------------------------------------- INICIO SEGURANÇA --------------------------------------------
     * Apresenta caso o módulo seja Segurança
     */
    function seguranca(){
        global $db;
        $sql = "SELECT sisid FROM seguranca.sistema WHERE sisstatus = 'A' AND sisid != 999";
        $dados = $db->carregar ( $sql );
        $dados = $dados ? $dados : array();
        foreach ( $dados as $sisidArr ) {
            $sisid = $sisidArr ['sisid'];
            $sql = sprintf ( "SELECT s.sisid,  s.sisdsc, s.sisdiretorio	FROM seguranca.sistema s WHERE sisid = %d", $sisid );
            $sistema = ( object ) $db->pegaLinha ( $sql );

            $sql = sprintf ( "SELECT us.suscod , us.pflcod , p.pfldsc FROM seguranca.usuario_sistema us
                LEFT JOIN seguranca.perfil p USING ( pflcod ) WHERE	 us.sisid = %d AND usucpf = '%s'", $sistema->sisid, $usucpf );
            $usuariosistema = ( object ) $db->pegaLinha ( $sql );
            $sistema->usuariosistema = $usuariosistema;
            $sistemas [] = $sistema;

            // ----------- verificando se o modulo possui as tabelas necessarias e alguma porposta para que seja exibido a lista de propostas
            $sistema->sisdiretorio = $sistema->sisid == 14 ? 'cte' : $sistema->sisdiretorio;
            $sql = sprintf ( "SELECT CASE WHEN (SELECT true FROM pg_tables WHERE  schemaname='%s' AND
                    tablename = 'tiporesponsabilidade')	THEN true WHEN  (SELECT true FROM
                    pg_tables WHERE schemaname='%s' AND tablename = 'tprperfil')THEN true ELSE false END;",
            strtolower ( $sistema->sisdiretorio ), strtolower ( $sistema->sisdiretorio ));
            $existTable = $db->pegaUm ( $sql );
            if ($existTable == 't') {

                $sql_urp = "select urpcampo, pflcod from seguranca.usuariorespproposta where sisid = '" . $sisid . "' and usucpf = '" . $usucpf . "'";
                $urp = $db->pegaLinha ( $sql_urp );
                if ($urp) {

                    // $sql_perfil = "select pfldsc from seguranca.perfil where pflcod = ".$urp['pflcod']."";
                    // $dados_perfil = $db->pegaUm($sql_perfil);
                    $sql_tpresp = "select tprdsc, tprtabela, tprcampo, tprcampodsc from " . $sistema->sisdiretorio . ".tiporesponsabilidade where tprcampo = '" . $urp ['urpcampo'] . "'";
                    $dados_tpresp = $db->pegaLinha ( $sql_tpresp );

                    $sql_propostas = "select distinct urpcampoid, urpcampo
                        from seguranca.usuariorespproposta as urp
                        where sisid = '" . $sisid . "' and usucpf = '" . $usucpf . "'";
                    $dados_propostas = $db->carregar ( $sql_propostas );
                    
                    ?>

                <section class="form-group">
                    <section class="col-md-offset-2 col-md-10">Proposto</section>
                </section>					
                <section class="form-group">
                    <label class="control-label col-md-2">Perfil:</label>
                    <section class="col-md-10">
                        <?= $usuariosistema->pfldsc?>
                    </section>
                </section>

                <section class="form-group">
                    <label class="control-label col-md-2"><? echo($dados_tpresp['tprdsc'].": "); ?></label>
                    <section class="col-md-10">
                        <table class="table table-bordered table-hover" >
                            <tr>
                                <td width="100%" bgcolor="#e9e9e9" align="left">Código - <?=$dados_tpresp['tprdsc'] ?></td>
                            </tr>
                            <?php
                            $count = 0;
                            $dados_propostas = $dados_propostas ? $dados_propostas : array();
                            foreach ( $dados_propostas as $dado ) {										
                                $sql_proposta = "select distinct
                                    " . $dados_tpresp ['tprcampodsc'] . " as descricao
                                    from " . $dados_tpresp ['tprtabela'] . "
                                    where " . $dados_tpresp ['tprcampo'] . " = '" . $dado ['urpcampoid'] . "'";

                                $dados_ptoposta = $db->pegaLinha ( $sql_proposta );										
                                if ($count % 2)
                                    $backcolor = "class=\"active\"";
                                else
                                    $backcolor = "";
                                    $count ++;
                                echo ("<tr " . $backcolor . "><td>" . $dado ['urpcampoid'] . " - " . $dados_ptoposta ['descricao'] . "</td></tr>");
                            }
                            ?>
                        </table>
                    </section>
                </section>
            <?php
                }
            } ?>

                <section class="form-group">
                    <section class="col-md-12">
                        <h4 style="border-bottom: 1px solid #ccc;">Atribuído</h4>
                    </section>												
                </section>
                <section class="form-group">
                    <label class="control-label col-md-2">Sistema:</label>
                    <section class="col-md-10"><b><?= $sistema->sisdsc ?></b></section>
                </section>
                <section class="form-group">
                    <label class="control-label col-md-2">Status:</label>
                    <section class="col-md-10">
                        <section class="btn-group" data-toggle="buttons">                            
                            <input id="status_ativo_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="A" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );"
                                <?= $usuariosistema->suscod == 'A' ? 'checked="checked"' : "" ?> />					
                            <label for="status_ativo_<?= $sistema->sisid ?>">Ativo</label> 

                            <input id="status_pendente_<?= $sistema->sisid ?>" type="radio"	name="status[<?= $sistema->sisid ?>]" value="P" onchange="alterar_status_sistema( <?= $sistema->sisid ?> );"
                                <?= $usuariosistema->suscod == 'P' ? 'checked="checked"' : "" ?> />
                            <label for="status_pendente_<?= $sistema->sisid ?>">Pendente</label>

                            <input id="status_bloqueado_<?= $sistema->sisid ?>" type="radio" name="status[<?= $sistema->sisid ?>]" value="B"
                                onchange="alterar_status_sistema( <?= $sistema->sisid ?> );"
                                <?= $usuariosistema->suscod == 'B' ? 'checked="checked"' : "" ?> />
                            <label for="status_bloqueado_<?= $sistema->sisid ?>">Bloqueado</label>								
                        </section>
                        <a class="btn btn-default" href="javascript: exibir_ocultar_historico('historico_<?= $sistema->sisid ?>');">
                            <img src="/imagens/mais.gif" style="border: 0" /> histórico
                        </a>
                    </section>
                    <section class="col-md-10 col-md-offset-2">
                        <div id="historico_<?= $sistema->sisid ?>" style="margin-top:20px; display: none">
                            <p>
                            <?php
                                $cabecalho = array (
                                "Data",
                                "Status",
                                "Descrição",
                                "CPF" 
                                );
                                $sql = sprintf ( "
                                    SELECT 
                                        to_char( hu.htudata, 'dd/mm/YYYY' ) as data,
                                        hu.suscod,
                                        hu.htudsc,
                                        hu.usucpfadm
                                    FROM seguranca.historicousuario hu
                                    WHERE usucpf = '%s' 
                                        AND sisid = %d
                                    ORDER BY hu.htudata DESC", $usucpf, $sistema->sisid );
                                $db->monta_lista_simples ( $sql, $cabecalho, 25, 0 );
                            ?>
                            </p>
                        </div>
                    </section>
                </section>
                <section class="form-group">
                    <label class="control-label col-md-2">Justificativa:</label>
                    <section class="col-md-10">
                        <div id="justificativa_on_<?= $sistema->sisid ?>"style="display: none;">
                            <?php inputTextArea('justificativa['.$sistema->sisid.']', $valor, 'justificativa['. $sistema->sisid .']', '3000',array('cols'=>100));?>
                        </div>
                        <div id="justificativa_off_<?= $sistema->sisid ?>"style="display: block; color: #909090;" class="form-control-static">
                            Status não alterado.
                        </div>
                    </section>
                </section>
            <?php 
            if ( $usuariosistema->pflcod &&  ( ! $urp )){ ?>
                <section class="form-group">
                    <label class="control-label col-md-2">Perfil Desejado:</label>
                    <section class="col-md-10">
                        <?= $usuariosistema->pfldsc ?>
                    </section>
                </section>
            <?php     
            } ?>
            <section class="form-group">
                <label class="control-label col-md-2">Perfil:</label>
                <section class="col-md-10">
                <?php
                $sql = sprintf ( "
                    SELECT
                        p.pflnivel
                    FROM seguranca.perfil p 
                    INNER JOIN seguranca.perfilusuario pu on pu.pflcod = p.pflcod 
                        AND pu.usucpf = '%s' 
                        AND p.sisid = %d
                    ORDER BY p.pflnivel", $_SESSION ['usucpf'], $sistema->sisid );
                $nivel = $db->pegaUm ( $sql );
                $nivel = 0;
                $sql_perfil = sprintf ( "
                    SELECT DISTINCT
                        p.pflcod as codigo,
                        p.pfldsc as descricao
                    FROM seguranca.perfil p 
                    LEFT JOIN seguranca.perfilusuario pu on pu.pflcod=p.pflcod
                    WHERE p.pflstatus='A' 
                        AND p.sisid=%d
                    ORDER BY descricao", $nivel, $sistema->sisid );
                $sql = sprintf ( "
                    SELECT DISTINCT 
                        p.pflcod as codigo,
                        p.pfldsc as descricao
                    FROM seguranca.perfilusuario pu 
                    INNER JOIN seguranca.perfil p on p.pflcod=pu.pflcod
                    WHERE p.pflstatus = 'A' 
                        AND p.sisid = %d 
                        AND pu.usucpf = '%s'
                    ORDER BY descricao", $sistema->sisid, $usucpf );
                $nome = 'pflcod[' . $sistema->sisid . ']';
                $$nome = $db->carregar ( $sql );
                combo_popup ( 'pflcod[' . $sistema->sisid . ']', $sql_perfil, 'Selecione o(s) Perfil(s)', '360x460' );
                ?>
                </section>
            </section>
            <?php $sistema->sisdiretorio = $sistema->sisid == 14 ? 'cte' : $sistema->sisdiretorio;
            $sql = sprintf ( "  
                SELECT 
                    CASE
                        WHEN
                            (SELECT true FROM pg_tables WHERE schemaname='%s' AND tablename = 'tiporesponsabilidade')
                        THEN true
                        WHEN
                            (SELECT true FROM pg_tables WHERE schemaname='%s' AND tablename = 'tprperfil')
                        THEN true
                        ELSE false
                    END;", $sistema->sisdiretorio, $sistema->sisdiretorio );
            $existTable = $db->pegaUm ( $sql );
            unset ( $sql );
            if ($existTable === 't' && $sistema->sisdiretorio != 'pde'){
                $sql = "SELECT tprsigla, tprurl FROM " . $sistema->sisdiretorio . ".tiporesponsabilidade
                        WHERE tprsnvisivelperfil = 't' ORDER BY tprdsc";
                $responsabilidades = ( array ) $db->carregar ( $sql );
                $sqlPerfisUsuario = "   
                    SELECT p.pflcod, p.pfldsc FROM seguranca.perfil p
                    INNER JOIN seguranca.perfilusuario pu ON pu.pflcod = p.pflcod AND pu.usucpf = '%s' and sisid=" . $sistema->sisid . "
                    WHERE p.pflstatus='A' ORDER BY p.pfldsc";
                $query = sprintf ( $sqlPerfisUsuario, $usucpf );
                $perfisUsuario = $db->carregar ( $query );
                $script .= gerFuncResp ( $sistema->sisid, $sistema->sisdiretorio, $usucpf, $responsabilidades );
                
                if( $perfisUsuario ){ ?>
                    <section class="form-group">
                        <label class="control-label col-md-2">Associação de Perfil:</label>
                        <section class="col-md-10">
                            <table class="table table-bordered table-hover">
                                <tr>
                                    <td width="12" rowspan="2" bgcolor="#e9e9e9" align="center">&nbsp;</td>
                                    <td rowspan="2" align="left" bgcolor="#e9e9e9" align="center">Descrição</td>
                                    <td align="center" colspan="<?=@count($responsabilidades)?>" bgcolor="#e9e9e9" align="center" style="border-bottom: 1px solid #bbbbbb">Responsabilidades</td>
                                </tr>
                                <tr>
                                <?php $responsabilidades = $responsabilidades ? $responsabilidades  : array();
                                    foreach( $responsabilidades as $responsabilidade ){ ?>
                                    <td align="center" bgcolor="#e9e9e9" align="center"><?= $responsabilidade["tprdsc"] ?></td>
                                <?php  } ?>
                                </tr>
                                <?php $perfisUsuario = $perfisUsuario ? $perfisUsuario  : array();
                                foreach( $perfisUsuario as $perfil ){
                                    $marcado = $i ++ % 2 ? '#F7F7F7' : '';
                                    $sqlResponsabilidadesPerfil = "
                                        SELECT p.*, tr.tprdsc, tr.tprsigla FROM
                                            (SELECT * FROM " . $sistema->sisdiretorio . ".tprperfil
                                            WHERE pflcod = '%s') p 
                                        RIGHT JOIN " . $sistema->sisdiretorio . ".tiporesponsabilidade tr ON p.tprcod = tr.tprcod
                                        WHERE tprsnvisivelperfil = TRUE ORDER BY tr.tprdsc";
                                    $query = sprintf ( $sqlResponsabilidadesPerfil, $perfil ["pflcod"] );
                                    $responsabilidadesPerfil = ( array ) $db->carregar ( $query );
                                    // Esconde a imagem + para perfis sem responsabilidades
                                    $mostraMais = false;
                                    $responsabilidadesPerfil = $responsabilidadesPerfil ? $responsabilidadesPerfil  : array();
                                    foreach ( $responsabilidadesPerfil as $resPerfil ) {
                                        if (( boolean ) $resPerfil ["tprcod"]) {
                                            $mostraMais = true;
                                            break;
                                        }
                                    }
                                    ?>

                                <tr bgcolor="<?=$marcado?>">
                                    <td style="color: #003c7b">
                                <?php
                                    if ($mostraMais){ ?>											
                                        <a href="Javascript:abreconteudo('../<?= $sistema->sisdiretorio; ?>/geral/cadastro_responsabilidades.php?usucpf=<?=$usucpf?>&pflcod=<?=$perfil["pflcod"]?>','<?=$perfil["pflcod"]?>','<?=$usucpf?>')">
                                            <img src="../imagens/mais.gif" name="+" border="0"	id="img<?=$perfil["pflcod"]?>" />
                                        </a>
                                    <?php
                                    } ?>
                                    </td>
                                    <td><?=$perfil["pfldsc"]?></td>
                                <?php
                                    foreach( $responsabilidadesPerfil as $resPerfil ){ ?>
                                    <td align="center">
                                <?php
                                        if ((boolean) $resPerfil["tprcod"] ){ ?>
                                        <input type="button" name="btnAbrirResp<?=$perfil["pflcod"]?>" value="Atribuir"	onclick="popresp_<?= $sistema->sisid ?>(<?=$perfil["pflcod"]?>, '<?=$resPerfil["tprsigla"]?>')">
                                        <?php                                                 
                                        }else{ ?>
                                        -
                                        <?php                                                
                                        } ?>
                                    </td>
                                    <?php
                                    } ?>
                                </tr>
                                <tr bgcolor="<?=$marcado?>">
                                    <td colspan="10" id="td<?=$perfil["pflcod"]?>"></td>
                                </tr>
                                <?php
                                } ?>
                            </table>
                        </section>
                    </section>
                <?php
                }				
            }
        }					
    }		
    //------------------------------------------- FIM DA FUNÇÃO SEGURANÇA -----------------------------------------------------
}
?>