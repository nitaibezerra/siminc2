<style>
    #quadro{

    }
    .botao_par{
        padding: 20px;
        float: left;
        cursor: pointer;
        margin: 5px;
        border: solid black 1px;
        background-color: #f5f5f5;
        width:140px;
        height:105px;
    }
    .botao_par2{
        padding: 20px;
        float: left;
        cursor: pointer;
        margin: 5px;
        border: solid black 1px;
        background-color: #f5f5f5;
        width:140px;
        height:105px;
    }
</style>

<?php
	/*
	header('Content-type: text/html; charset="iso-8859-1"',true);
	header("Cache-Control: no-store, no-cache, must-revalidate");// HTTP/1.1
	header("Cache-Control: post-check=0, pre-check=0", false);
	header("Pragma: no-cache");// HTTP/1.0 Canhe Livre
	*/

	unset($_SESSION['par']['adpid']);
	unset($_SESSION['par']['prgid']);
	unset($_SESSION['par']['pfaid']);
	unset($_SESSION['par']['pfaano']);

	$muncod = $_SESSION['par']['muncod'];
	$obPreObraControle = new PreObraControle();
	$nrAdesao = $obPreObraControle->verificaPrazoExpiraAdesao($muncod);
	$boAdesao = $nrAdesao > 0 ? true : false;


	/*
	 *Código comentado abaixo, devido mudança nas regras:
	 *Regra anterior: É visivel apenas os programas que estão dentro da data abil de adesao.
	 *Regra nova: definida em 28/07/2012, agora é possivel visualizar todos os programas, no entanto, so sera possivel a ediçao nos programas que estao em data de adesao habil.
	 *Porgramas que não estao em data abil para adesao so sera possivel visualizar os dados.
	 *autor da alteraçao: Luciano F. Ribeiro data:28/07/2012 e revisado em: 01/08/2012.
         *
         */
        
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	$wherePrograma = "";

    if($_SESSION['par']['itrid']){
        $possuiPendenciaObras = $_SESSION['par']['itrid'] == 2 ? getObrasPendentesPAR($_SESSION['par']['muncod']) : getObrasPendentesPAR(null, $_SESSION['par']['estuf']);
    }
	//Verifica se o Perfil é Analista Programas MEC e qual programa o usuário pode acessar
	$boHabilitaPrograma = 'true';
	if( in_array(PAR_PERFIL_PROFUNC_ANALISEPF, $perfil) ){
            $sql_analisepf = "
                SELECT  pfa.pfaid
                FROM par.usuarioresponsabilidade usu

                INNER JOIN par.programa prg ON prg.prgid = usu.prgid
                INNER JOIN par.pfadesao pfa ON pfa.prgid = prg.prgid
                WHERE usu.rpustatus='A' AND usu.usucpf = '{$_SESSION['usucpf']}'
            ";
            $rs_analisepf = $db->carregarColuna($sql_analisepf);

            if( $rs_analisepf ){
                $analisepf = implode (',',$rs_analisepf);
                $wherePrograma = " AND pa.pfaid IN({$analisepf})";
            } else {
                $boHabilitaPrograma = 'false';
            }
	}

	$sql = "
            SELECT  pa.pfaid,
                    pa.prgid,
                    pr.prgdsc,
                    pa.pfadatainicial,
                    pa.pfadatafinal,
                    pa.pfaano,
                    pa.pfaicone,
                    pa.pfaesfera,
                    pfabloqueioobras
            FROM par.pfadesao pa

            LEFT JOIN par.programa pr ON pr.prgid = pa.prgid
            WHERE pa.pfastatus = 'A'

            $wherePrograma AND pfaid NOT IN (4)

            ORDER BY 3
        ";
	$ProgramaCursista = $db->carregar($sql);

	$boData = true;
	if( verificaGrupoMunicipioMUNCOD( $muncod ) ){
            if(date('YmdHis') <= DATA_EXPIRA_PROINFANCIA_PENDENCIAS){
                $boData = true;
            }
	}else{
            if(date('YmdHis') <= DATA_EXPIRA_PROINFANCIA){
                $boData = true;
            }
	}
        echo "<br>";

	//VERIFICAR COTAS PROINFÂNCIA 2014
	$cotaproinfancia = verificarQtdObraProinfancia();
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="5" align="center" style="width:98%">
    <tr>
        <td><input type="hidden" name="bohabilitaprograma" id="bohabilitaprograma" value="<?= $boHabilitaPrograma; ?>">
            <div id="quadro">
                <?php if (!empty($cotaproinfancia)) { ?>
                    <div id="novo_proinfancia" name="M" class="botao_par" title="<?php echo $_SESSION['par']['itrid'] ?>"><img src="../imagens/simbolo_pro_infancia_novo_2014.gif" /></div>
                <?php }
                
                    if ($_SESSION['par']['muncod'] != '') {
                        if ($boAdesao || $boData) {
                ?>
                            <div id="principal/programas/proinfancia/proInfancia" name="M" class="botao_par" title="<?php echo $_SESSION['par']['itrid'] ?>">
                                <img src="../imagens/simbolo_pro_infancia_novo.gif" />
                            </div>
                <?php
                        } else {
                            if( !in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) &&
                                !in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) &&
                                !in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) &&
                                !in_array(PAR_PERFIL_COORDENADOR_TECNICO, $perfil) ) {
                ?>
                                <div id="expirou" class="botao_par"><img src="../imagens/simbolo_pro_infancia_novo.gif" /></div>
                <?php
                            } else {
                ?>
                                <div id="principal/programas/proinfancia/proInfancia" name="M" class="botao_par" title="<?php echo $_SESSION['par']['itrid'] ?>"><img src="../imagens/simbolo_pro_infancia_novo.gif" /></div>
                <?php
                            }
                        }

                        $muncodsPronatec = $db->carregarColuna("SELECT DISTINCT muncod FROM par.termopronatec");

                        $arrPflNotPronatec = Array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, PAR_PERFIL_PREFEITO);

                        $arrComparacao = array_intersect($perfil, $arrPflNotPronatec);

                        if (in_array($_SESSION['par']['muncod'], $muncodsPronatec) && $arrComparacao[0] == '') {
                ?>
                            <div id="principal/programas/pronatec/pronatec" class="botao_par"><img src="../imagens/rede_federal.jpg" /></div>

                <?php
                        }
                    }else {
                        if ($_SESSION['par']['estuf'] == 'DF') {
                ?>
                            <div id="principal/programas/proinfancia/proInfancia" name="P" class="botao_par" title="<?php echo $_SESSION['par']['itrid'] ?>"><img src="../imagens/simbolo_pro_infancia_novo.gif" /></div>
                <?php
                        }
                ?>
                    <div id="principal/programas/proinfancia/proInfancia" name="Q" class="botao_par" style="font-size:30px">
                        <img src="../imagens/par/construcao_de_quadras.jpg" height="105 px"/>
                    </div>

                    <div id="principal/programas/proinfancia/proInfancia" name="C" class="botao_par" style="font-size:30px">
                        <img src="../imagens/par/cobertura_de_quadras.jpg" height="105 px"/>
                    </div>
                <?PHP
                        if ($_SESSION['par']['estuf'] == 'DF') {
                ?>
                            <div id="principal/programas/feirao_programas/termoadesao" title="5" class="botao_par" style="font-size:30px">
                                <img src="../imagens/simbolo_pro_veiculos_acessiveis.jpg" height="105 px"/>
                            </div>
                <?PHP
                        }	
                    }

                if ($ProgramaCursista) {
                    foreach ($ProgramaCursista as $p) {
                        $mostra = "N";

                        // Verifica se há pendências de obras para o município/estado e está setado para bloquear
                        $bloqueioObras = (($p['pfabloqueioobras'] == 't') && !empty($possuiPendenciaObras)) ? 'bloqueio="sim"' : 'bloqueio="nao"';

                        $sql = "SELECT muncod, estuf FROM par.municipioadesaoprograma WHERE pfaid = " . $p['pfaid'];
                        $dados = $db->carregar($sql);
 
                        if ($p['pfaesfera'] == 'M') {
                            #REGRA SOMENTE PARA O PROGRAMA EJA PRONATEC.
                            if( $p['prgid'] == PROG_PAR_EJA_PRONATEC ) {
                                
                                $sql = "SELECT codigoibge AS muncod FROM eja.ejacruzamento WHERE codigoibge = '{$_SESSION['par']['muncod']}';";
                                $muncod = $db->pegaUm($sql);
                                
                                if( $muncod != '' ){
                                    $mostra = "S";
                                }else{
                                    $mostra = "N";
                                }
                             
                            }else
                                if( $p['prgid'] == PROG_PAR_MAIS_MEDICOS ) {
                                #CÓDIGO IBGE NA BASE ESTA COM 6 DIGITOS.
                                $sql = "SELECT munbloqid FROM maismedicomec.municipiobloqueio WHERE muncodigo = '".substr($_SESSION['par']['muncod'], 0, 6)."';";
                                $muncod = $db->pegaUm($sql);
                                
                                if( $muncod > 0 ){
                                    $mostra = "N";
                                }else{
                                    $mostra = "S";
                                }
                                
                            }else
                                if( $p['prgid'] == PROG_PAR_MAIS_MEDICO_NOVO_2015 ) {
                                #CÓDIGO IBGE NA BASE ESTA COM 6 DIGITOS.
                                $sql = "SELECT munbloqid FROM maismedicomec.municipiobloqueio WHERE muncodigo = '".substr($_SESSION['par']['muncod'], 0, 6)."';";
                                $muncod = $db->pegaUm($sql);
                                
                                if( $muncod > 0 ){
                                    $mostra = "S";
                                }else{
                                    $mostra = "N";
                                }
                                
                            } else {
                                if ($dados[0]['muncod'] == "") {
                                    $mostra = "S";
                                } else {
                                    foreach ($dados as $d) {
                                        if ($_SESSION['par']['muncod'] == $d['muncod']) { //ver('entro', d);
                                            $mostra = "S";
                                        }
                                    }
                                }
                            }
                            
                        } elseif ($p['pfaesfera'] == 'E') {
                            if ($dados[0]['estuf'] == "") {
                                $mostra = "S";
                            } else {
                                foreach ($dados as $d) {
                                    if ($_SESSION['par']['estuf'] == $d['estuf']) {
                                        $mostra = "S";
                                    }
                                }
                            }
                        } elseif ($p['pfaesfera'] == 'T') {
                            $esfera = $_SESSION['par']['itrid'] == 2 ? 'M' : 'E';

                            if ($esfera == 'M') {                                
                                if ($dados[0]['muncod'] == "") {
                                    $mostra = "S";
                                } else {
                                    foreach ($dados as $d) {
                                        if ($_SESSION['par']['muncod'] == $d['muncod']) {
                                            $mostra = "S";
                                        }
                                    }
                                }
                            } else {
                                if ($dados[0]['estuf'] == "") {
                                    $mostra = "S";
                                } else {
                                    foreach ($dados as $d) {
                                        if ($_SESSION['par']['estuf'] == $d['estuf']) {
                                            $mostra = "S";
                                        }
                                    }
                                }
                            }
                        }

                        if ($mostra == "S") {

                            $dirname = APPRAIZ . "arquivos/proinfantil/feirao_programas/";
                            $filename = $dirname . $p['pfaicone'];

                            if (!file_exists($filename)) {
                                $filename = "../imagens/{$p['pfaicone']}";
                            }

                            if ($p['prgid'] == 60) {
                                if (!$_SESSION['par']['muncod'] || !$wherePrograma) { //muncod = null -> acesso liberado somente para estado/uf.
                                    if ($p['pfaicone'] != 'X') {
                                        echo '<div id="principal/programas/feirao_programas/termoadesao" class="botao_par" ' . $bloqueioObras . ' title="' . $p['pfaid'] . '"><img src="' . $filename . '"></div>';
                                    } else {
                                        echo '<div id="principal/programas/feirao_programas/termoadesao" class="botao_par" ' . $bloqueioObras . ' title="' . $p['pfaid'] . '">' . $p['prgdsc'] . '</div>';
                                    }
                                }
                            } elseif ($p['prgid'] == 4) {
                                $aryEstadosForas = array('DF', 'GO', 'MT', 'MG', 'RJ');

                                if (( $_SESSION['par']['itrid'] == 1 && !in_array($_SESSION['par']['estuf'], $aryEstadosForas) ) &&
                                    ($_SESSION['par']['itrid'] !== 2) && (
                                        in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
                                        in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
                                        in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
                                        in_array(PAR_PERFIL_PROFUNC_PREANALISEPF, $perfil)
                                    )
                                ) {
                                    echo '<div id="principal/programas/feirao_programas/termoadesao" class="botao_par" ' . $bloqueioObras . ' title="' . $p['pfaid'] . '"><img src="' . $filename . '"></div>';

                                } else if (in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) || in_array(PAR_PERFIL_PROFUNC_PREANALISEPF, $perfil)) {

                                    echo '<div id="principal/programas/feirao_programas/termoadesao" class="botao_par" ' . $bloqueioObras . ' title="' . $p['pfaid'] . '"><img src="' . $filename . '"></div>';
                                }

                            } else {

                                if ($p['pfaesfera'] == 'T') {
                                    echo '<div id="principal/programas/feirao_programas/termoadesao" class="botao_par" ' . $bloqueioObras . ' title="' . $p['pfaid'] . '"><img src="' . $filename . '"></div>';
                                } elseif ($p['pfaesfera'] == 'M' && $_SESSION['par']['muncod']) {
                                    echo '<div id="principal/programas/feirao_programas/termoadesao" class="botao_par" ' . $bloqueioObras . ' title="' . $p['pfaid'] . '"><img src="' . $filename . '"></div>';
                                } elseif ($p['pfaesfera'] == 'E' && !$_SESSION['par']['muncod']) {
                                    echo '<div id="principal/programas/feirao_programas/termoadesao" class="botao_par" ' . $bloqueioObras . ' title="' . $p['pfaid'] . '"><img src="' . $filename . '"></div>';
                                }
                            }
                        }
                    }
                }
                ?>

                <div title="100" class="botao_par2" id="plano_formacao" style="background-color: rgb(245, 245, 245); border: 1px solid black;">
                    <img src="../imagens/simbolo_snf1.JPG">
                </div>
            </div>
        </td>
    </tr>
</table>

<?php unset($_SESSION['maismedicos'], $_SESSION['continuaAdesao'], $_SESSION['par']['rqmid'], $_SESSION['entro_primeira_vez'], $_SESSION['total_matricula'], $_SESSION['continuaAdesaoPronatec'], $_SESSION['continuaAdesaoMaisMedico_2015'] ); ?>

<script>

    jQuery(function(){
	jQuery('#plano_formacao').click(function(){
            location.href = "../muda_sistema.php?sisid=122";
	});
    });

    jQuery('.botao_par').click(function(){

    if(jQuery(this).attr('bloqueio') == 'sim'){
        alert('Você não pode executar essa operação pois existe a necessidade de atualização dos dados no sistema de monitoramento de obras.');
        return false;
    }

	if(jQuery('#bohabilitaprograma').val() == 'false'){
		alert('Este programa não está vinculado ao seu usuário!');
		return false;
	} else {
            if(jQuery(this).attr('id') == 'principal/listaProfessores'){
                    window.location.href = 'par.php?modulo=' + jQuery(this).attr('id') + '&acao=A';
            }else
                if(jQuery(this).attr('id') == 'principal/programas/feirao_programas/termoadesao'){
                    window.location.href = 'par.php?modulo=' + jQuery(this).attr('id') + '&acao=A' + '&pfaid=' + this.title;
            }else 
                if(jQuery(this).attr('id') == 'principal/programas/pronatec/pronatec'){
                    window.location.href = 'par.php?modulo=' + jQuery(this).attr('id') + '&acao=A';
            }else 
                if(jQuery(this).attr('id') == 'principal/programas/feirao_programas/professortutor'){
                    window.location.href = 'par.php?modulo=' + jQuery(this).attr('id') + '&acao=A';
            }else 
                if(jQuery(this).attr('id') == 'novo_proinfancia'){
                    window.location.href = 'par.php?modulo=principal/programas/proinfancia/proInfancia&acao=A&programa=proinfancia2014';
            }else 
                if(this.title == 2){
                    window.location.href = 'par.php?modulo=' + jQuery(this).attr('id') + '&acao=A';
            }else 
                if(this.id == 'expirou'){
                    alert("Encerrado prazo para inscrições em 29/10/2010.");
            }else{
                var param = '';
                if(jQuery(this).attr('name')!=''){param = '&tipo='+jQuery(this).attr('name');}
                window.location.href = 'par.php?modulo=' + jQuery(this).attr('id') + '&acao=A'+param;
            }
	}	
    });

    jQuery('.botao_par').mouseover(function(){
	jQuery(this).css('background-color', '#e9e9e9');
	jQuery(this).css('border', 'solid #cdcdcd 1px');

	jQuery(this).mouseout(function(){
            jQuery(this).css('background-color', '#f5f5f5');
            jQuery(this).css('border', 'solid black 1px');
	});
    });

</script>