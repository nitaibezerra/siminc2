<?php
include APPRAIZ . "includes/cabecalho.inc";
require (APPRAIZ . 'includes/library/simec/Listagem.php');
$perfis = pegaPerfilGeral();
$dataAtual = date('Y/m/d');
$dataLimite = date('2015/10/01');
$cpf = $_SESSION['usucpf'];


//Listagem da escola relacionada ou diretor
if (in_array(PERFIL_EJA_DIRETOR_ESCOLA, $perfis) && !$_REQUEST['requisicao']) {
    $sqlDados = "SELECT
								 DISTINCT
                                                                 ent.entid as entid_diretor,
                                                                 ent.entcodent as codigo,
								 ent.entnome as nome,
								 ende.endlog || ' - '|| ende.endbai as endereco,
								 est.estuf as uf,
								 mun.mundescricao as mun, 
								mun.muncod
								FROM
								 entidade.entidade ent 
								 INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
								 --INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid -- AND ed.entpdeescola = 't'
								 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
								 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
								 INNER JOIN territorios.estado est ON est.estuf = mun.estuf
								 LEFT JOIN eja.usuarioresponsabilidade ur ON ur.entid = ent.entid AND
								 	rpustatus = 'A'	
								WHERE
								ur.usucpf = '{$_SESSION['usucpf']}' 
								ORDER BY
								 ent.entnome";
    $dadosP = $db->carregar($sqlDados);
    if ($dadosP[1]) {
            ?>
            <div class="page-header">
                <h4 id="forms">Confirmação de CPF</h4>   
            </div>
            <br><?php
            $sqlEscolas = "SELECT
							
								 '<a href=\"eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A&requisicao=listaTurmasDiretor&entid='||ent.entid||'\">'||ent.entnome||'</a>' as nome,
								 ende.endlog || ' - '|| ende.endbai as endereco,
								 est.estuf as uf,
								 mun.mundescricao as mun 
								FROM
								 entidade.entidade ent 
								 INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
								 --INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid -- AND ed.entpdeescola = 't'
								 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
								 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
								 INNER JOIN territorios.estado est ON est.estuf = mun.estuf
								 LEFT JOIN eja.usuarioresponsabilidade ur ON ur.entid = ent.entid AND
								 	rpustatus = 'A'	
								WHERE
								ur.usucpf = '{$_SESSION['usucpf']}' 
								ORDER BY ent.entnome";

            $listagem = new Simec_Listagem();
            $cabecalho = array(
                "Escola",
                "Endereço",
                "UF",
                "Município");
            $listagem->setCabecalho($cabecalho);
            $listagem->setQuery($sqlEscolas);
            $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

            if (false === $listagem->render()):
                ?>
                <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
                    Nenhum registro encontrado
                </div>
                <?php
            endif;
    }else {
        ?>
        <div class="page-header">
            <h4 id="forms">Confirmação de CPF</h4>   
        </div>
        <br>
        <div class="col-lg-12">
            <div class="well">
                <fieldset>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Código do INEP</label>
                        <div class="col-lg-10">
                            <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP[0]['codigo']; ?>"readonly="readonly" />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Nome da Escola</label>
                        <div class="col-lg-10">
                            <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP[0]['nome']; ?>"readonly="readonly" />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Endereço da Escola</label>
                        <div class="col-lg-10">
                            <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP[0]['endereco']; ?>"readonly="readonly" />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">UF</label>
                        <div class="col-lg-10">
                            <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP[0]['uf']; ?>"readonly="readonly" />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Município</label>
                        <div class="col-lg-10">
                            <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP[0]['mun']; ?>"readonly="readonly" />
                        </div>
                    </div>
                </fieldset>
            </div>
        </div>
        <br />
        <?PHP
//'<a href=\"pdeescola.php?modulo=meprincipal/meValidaEscolasPendentes&acao=A&entid='||et.entid||'&memid='||me.memid||'\" target=\"_blank\">'
        $sql = "SELECT CASE WHEN NTE.nteenviofnde2p is not null THEN '<a href=\"eja.php?modulo=principal/confirmarcpf/alunos_turma&acao=A&status='||NTE.nteenviofnde2p||'&nteid='||NTE.nteid||'&entid='||pk_cod_entidade||'\">
Turma '||NTE.ntesequencial||'</a>' ELSE '<a href=\"eja.php?modulo=principal/confirmarcpf/alunos_turma&acao=A&nteid='||NTE.nteid||'&entid='||pk_cod_entidade||'\">
Turma '||NTE.ntesequencial||'</a>' END AS turma, nt.nivdescricao, CASE WHEN NTE.nteenviofnde2p = 'C'  THEN
'Turma Fechada - CPF Confirmado' WHEN NTE.nteenviofnde2p = 'V'  THEN
'Turma Fechada - CPF Confirmado' WHEN NTE.nteenviofnde2p = 'E'  THEN
'Turma Fechada - CPF Confirmado - Lote Gerado' ELSE
'Turma aberta para confirmação de CPF' end as situacao,
   -- CASE WHEN nteenviofnde = 'V'
                   -- THEN 
                   '<img src=\"/imagens/cadiado_p.png\" width=\"18px\">'
                   -- ELSE
                     --   CASE WHEN ( nte.ntefechaturma = 'A' )
                      --      THEN '<img src=\"/imagens/cadeadoAberto.png\" style=\"cursor: pointer;\" width=\"18px\" onclick=\"fechar(' || nte.nteid || ')\" >'
                      --      ELSE '<img src=\"/imagens/cadiado.png\" style=\"cursor: pointer;\" width=\"18px\" onclick=\"abrir(' || nte.nteid || ')\" >'
                      --  END
               -- END 
               AS ico_cadeado
  FROM EJA.NOVATURMAEJA NTE JOIN EJA.USUARIORESPONSABILIDADE USR ON USR.ENTID=NTE.PK_COD_ENTIDADE
  JOIN EJA.NIVELTURMA NT ON NTE.NIVID = NT.NIVID
  WHERE USR.USUCPF = '{$_SESSION['usucpf']}' 
 AND NTE.nteenviofnde = 'E'
 AND usr.rpustatus = 'A'
 AND pk_cod_entidade = '{$dadosP[0]['entid_diretor']}'
order by NTE.NTESEQUENCIAL";

        $listagem = new Simec_Listagem();
        $cabecalho = array(
            "Turma",
            "Nivel",
            "Situação",
            "Validar CPF/Turma");
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        ?>

        <?php if (false === $listagem->render()): ?>
            <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
                Nenhum registro encontrado
            </div>
            <?php
        endif;
    }
}

//Listagem da escola relacionada ou diretor
if (in_array(PERFIL_EJA_DIRETOR_ESCOLA, $perfis) && $_REQUEST['requisicao'] == 'listaTurmasDiretor') {
    $sqlDados = "SELECT
								 DISTINCT
                                                                 ent.entid as entid_diretor,
                                                                 ent.entcodent as codigo,
								 ent.entnome as nome,
								 ende.endlog || ' - '|| ende.endbai as endereco,
								 est.estuf as uf,
								 mun.mundescricao as mun, 
								mun.muncod
								FROM EJA.NOVATURMAEJA NTE 
								 inner join entidade.entidade ent on nte.pk_cod_entidade = ent.entid
								 --INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid -- AND ed.entpdeescola = 't'
								 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
								 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
								 INNER JOIN territorios.estado est ON est.estuf = mun.estuf
								 LEFT JOIN eja.usuarioresponsabilidade ur ON ur.entid = ent.entid AND
								 	rpustatus = 'A'	
								WHERE
								ur.usucpf = '{$_SESSION['usucpf']}' 
                                                                and pk_cod_entidade = '{$_REQUEST['entid']}'
								ORDER BY
								 ent.entnome";
    $dadosP = $db->pegaLinha($sqlDados);
   
        ?>
        <div class="page-header">
            <h4 id="forms">Confirmação de CPF</h4>   
        </div>
        <br>
        <div class="col-lg-12">
            <div class="well">
                <fieldset>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Código do INEP</label>
                        <div class="col-lg-10">
                            <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['codigo']; ?>"readonly="readonly" />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Nome da Escola</label>
                        <div class="col-lg-10">
                            <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['nome']; ?>"readonly="readonly" />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Endereço da Escola</label>
                        <div class="col-lg-10">
                            <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['endereco']; ?>"readonly="readonly" />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">UF</label>
                        <div class="col-lg-10">
                            <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['uf']; ?>"readonly="readonly" />
                        </div>
                    </div>
                    <div class="form-group">
                        <label for="inputUnidade" class="col-lg-2 control-label">Município</label>
                        <div class="col-lg-10">
                            <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['mun']; ?>"readonly="readonly" />
                        </div>
                    </div>
                </fieldset>
            </div>
        </div>
        <br />
        <?PHP
//'<a href=\"pdeescola.php?modulo=meprincipal/meValidaEscolasPendentes&acao=A&entid='||et.entid||'&memid='||me.memid||'\" target=\"_blank\">'
        $sql = "SELECT CASE WHEN NTE.nteenviofnde2p is not null THEN '<a href=\"eja.php?modulo=principal/confirmarcpf/alunos_turma&acao=A&status='||NTE.nteenviofnde2p||'&nteid='||NTE.nteid||'&entid='||pk_cod_entidade||'\">
Turma '||NTE.ntesequencial||'</a>' ELSE '<a href=\"eja.php?modulo=principal/confirmarcpf/alunos_turma&acao=A&nteid='||NTE.nteid||'&entid='||pk_cod_entidade||'\">
Turma '||NTE.ntesequencial||'</a>' END AS turma, nt.nivdescricao, CASE WHEN NTE.nteenviofnde2p = 'C'  THEN
'Turma Fechada - CPF Confirmado' WHEN NTE.nteenviofnde2p = 'V'  THEN
'Turma Fechada - CPF Confirmado' WHEN NTE.nteenviofnde2p = 'E'  THEN
'Turma Fechada - CPF Confirmado - Lote Gerado' ELSE
'Turma aberta para confirmação de CPF' end as situacao,
   -- CASE WHEN nteenviofnde = 'V'
                   -- THEN 
                   '<img src=\"/imagens/cadiado_p.png\" width=\"18px\">'
                   -- ELSE
                     --   CASE WHEN ( nte.ntefechaturma = 'A' )
                      --      THEN '<img src=\"/imagens/cadeadoAberto.png\" style=\"cursor: pointer;\" width=\"18px\" onclick=\"fechar(' || nte.nteid || ')\" >'
                      --      ELSE '<img src=\"/imagens/cadiado.png\" style=\"cursor: pointer;\" width=\"18px\" onclick=\"abrir(' || nte.nteid || ')\" >'
                      --  END
               -- END 
               AS ico_cadeado
  FROM EJA.NOVATURMAEJA NTE JOIN EJA.USUARIORESPONSABILIDADE USR ON USR.ENTID=NTE.PK_COD_ENTIDADE
  JOIN EJA.NIVELTURMA NT ON NTE.NIVID = NT.NIVID
  WHERE USR.USUCPF = '{$_SESSION['usucpf']}' 
 AND NTE.nteenviofnde = 'E'
 AND usr.rpustatus = 'A'
 AND pk_cod_entidade = '{$_REQUEST['entid']}'
order by NTE.NTESEQUENCIAL";

        $listagem = new Simec_Listagem();
        $cabecalho = array(
            "Turma",
            "Nivel",
            "Situação",
            "Validar CPF/Turma");
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        ?>

        <?php if (false === $listagem->render()): ?>
            <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
                Nenhum registro encontrado
            </div>
            <?php
        endif;
    }

if (in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $perfis) && (!$_REQUEST['requisicao'] || $_REQUEST['requisicao'] != 'listaTurmas')) {
    ?>
    <div class="page-header">
        <h4 id="forms">Confirmação de CPF</h4>   
    </div>
    <br><?php
    $sqlEscolas = "              SELECT
						                '<a href=\"eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A&requisicao=listaTurmas&entid='||ent.entid||'\">'||ent.entnome||'</a>' as nome,
								 ende.endlog || ' - '|| ende.endbai as endereco,
								 est.estuf as uf,
								 mun.mundescricao as mun 
								
								FROM
								 entidade.entidade ent 
								 INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
								 --INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid -- AND ed.entpdeescola = 't'
								 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
								 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
								 INNER JOIN territorios.estado est ON est.estuf = mun.estuf
								 LEFT JOIN eja.usuarioresponsabilidade ur ON ur.muncod = ende.muncod AND
								 	rpustatus = 'A'	
					where  ur.usucpf = '{$_SESSION['usucpf']}' AND fe.funid in (3,4) AND ent.entstatus='A'
								ORDER BY
								 ent.entnome ASC;";

    $listagem = new Simec_Listagem();
    $cabecalho = array(
        "Escola",
        "Endereço",
        "UF",
        "Município");
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sqlEscolas);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

    if (false === $listagem->render()):
        ?>
        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>
        <?php
    endif;
}
if ((in_array(PERFIL_EJA_GESTOR_MUNICIPAL, $perfis) || in_array(PERFIL_EJA_GESTOR_ESTADUAL, $perfis)) && ($_REQUEST['requisicao'] == 'listaTurmas' && !$_REQUEST['exec'])) {
    $sqlDados = "SELECT
								 DISTINCT
                                                                 ent.entcodent as codigo,
								 ent.entnome as nome,
								 ende.endlog || ' - '|| ende.endbai as endereco,
								 est.estuf as uf,
								 mun.mundescricao as mun 
								
								FROM
								 entidade.entidade ent 
								 INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
								 --INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid -- AND ed.entpdeescola = 't'
								 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
								 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
								 INNER JOIN territorios.estado est ON est.estuf = mun.estuf
								 LEFT JOIN eja.usuarioresponsabilidade ur ON ur.muncod = ende.muncod AND
								 	rpustatus = 'A'	
								WHERE
								--ur.usucpf = '{$_SESSION['usucpf']}' and 
                                                                ent.entid = '{$_REQUEST['entid']}'
								ORDER BY
								 ent.entnome ";
//                                                                ver($sqlDados);
    $dadosP = $db->PegaLinha($sqlDados);
    ?>
    <script>
        function validar(id) {
            window.location = 'eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A&requisicao=listaTurmas&exec=validar&entid=<?php echo $_REQUEST['entid'] ?>&nteid=' + id;
        }
        function desvalidar(id) {
            window.location = 'eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A&requisicao=listaTurmas&exec=desvalidar&entid=<?php echo $_REQUEST['entid'] ?>&nteid=' + id;
        }
    </script>
    <div class="page-header">
        <h4 id="forms">Confirmação de CPF</h4>   
    </div>
    <br>
    <div class="col-lg-12">
        <div class="well">
            <fieldset>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Código do INEP</label>
                    <div class="col-lg-10">
                        <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['codigo']; ?>"readonly="readonly" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Nome da Escola</label>
                    <div class="col-lg-10">
                        <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['nome']; ?>"readonly="readonly" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Endereço da Escola</label>
                    <div class="col-lg-10">
                        <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['endereco']; ?>"readonly="readonly" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">UF</label>
                    <div class="col-lg-10">
                        <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['uf']; ?>"readonly="readonly" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="inputUnidade" class="col-lg-2 control-label">Município</label>
                    <div class="col-lg-10">
                        <input type="text" class="CampoEstilo normal form-control" name="pi" id="pi" value="<?php echo $dadosP['mun']; ?>"readonly="readonly" />
                    </div>
                </div>
            </fieldset>
        </div>
    </div>
    <br />
    <?PHP
    if (strtotime($dataAtual) <= strtotime($dataLimite)) {
         $sql = "SELECT CASE WHEN NTE.nteenviofnde2p = 'C' THEN '<a href=\"eja.php?modulo=principal/confirmarcpf/alunos_turma&acao=A&entid='||nte.PK_COD_ENTIDADE||'&status='||NTE.nteenviofnde2p||'&nteid='||NTE.nteid||'\">
Turma '||NTE.ntesequencial||'</a>' ELSE '<a href=\"eja.php?modulo=principal/confirmarcpf/alunos_turma&acao=A&nteid='||NTE.nteid||'&entid='||nte.PK_COD_ENTIDADE||'\">
Turma '||NTE.ntesequencial||'</a>' END AS turma, nt.nivdescricao, CASE WHEN NTE.nteenviofnde2p = 'C'  THEN
'Turma Fechada - CPF Confirmado' WHEN NTE.nteenviofnde2p = 'V'  THEN
'Turma Fechada - CPF Confirmado' WHEN NTE.nteenviofnde2p = 'E'  THEN
'Turma Fechada - CPF Confirmado - Lote Gerado' ELSE
'Turma aberta para confirmação de CPF' end as situacao,
    CASE WHEN nteenviofnde2p = 'C'
                    THEN '<img src=\"/imagens/cadeadoAberto.png\" title=\"Turma não validada\" style=\"cursor: pointer;\" width=\"18px\" onclick=\"validar(' || nte.nteid || ')\" >'
                  WHEN nteenviofnde2p = 'V'
                    THEN  '<img src=\"/imagens/cadiado.png\" title=\"Turma validada\" style=\"cursor: pointer;\" width=\"18px\" onclick=\"desvalidar(' || nte.nteid || ')\" >'
                 WHEN nteenviofnde2p IS NULL 
                 THEN                    '<img src=\"/imagens/cadiado_p.png\" width=\"18px\">'

                END 
               AS ico_cadeado
  FROM EJA.NOVATURMAEJA NTE
  JOIN EJA.NIVELTURMA NT ON NTE.NIVID = NT.NIVID
  WHERE nte.PK_COD_ENTIDADE = '{$_REQUEST['entid']}' 
 AND NTE.nteenviofnde = 'E'
order by NTE.NTESEQUENCIAL;";
    }else{
         $sql = "SELECT CASE WHEN NTE.nteenviofnde2p = 'C' THEN '<a href=\"eja.php?modulo=principal/confirmarcpf/alunos_turma&acao=A&entid='||nte.PK_COD_ENTIDADE||'&status='||NTE.nteenviofnde2p||'&nteid='||NTE.nteid||'\">
Turma '||NTE.ntesequencial||'</a>' ELSE '<a href=\"eja.php?modulo=principal/confirmarcpf/alunos_turma&acao=A&nteid='||NTE.nteid||'&entid='||nte.PK_COD_ENTIDADE||'\">
Turma '||NTE.ntesequencial||'</a>' END AS turma, nt.nivdescricao, CASE WHEN NTE.nteenviofnde2p = 'C'  THEN
'Turma Fechada - CPF Confirmado' WHEN NTE.nteenviofnde2p = 'V'  THEN
'Turma Fechada - CPF Confirmado' WHEN NTE.nteenviofnde2p = 'E'  THEN
'Turma Fechada - CPF Confirmado - Lote Gerado' ELSE
'Turma aberta para confirmação de CPF' end as situacao,
    CASE WHEN nteenviofnde2p = 'C'
                    THEN '<img src=\"/imagens/cadeadoAberto.png\" title=\"Turma não validada\" width=\"18px\">'
                  WHEN nteenviofnde2p = 'V'
                    THEN  '<img src=\"/imagens/cadiado.png\" title=\"Turma validada\"  width=\"18px\" >'
                 WHEN nteenviofnde2p IS NULL 
                 THEN                    '<img src=\"/imagens/cadiado_p.png\" width=\"18px\">'

                END 
               AS ico_cadeado
  FROM EJA.NOVATURMAEJA NTE
  JOIN EJA.NIVELTURMA NT ON NTE.NIVID = NT.NIVID
  WHERE nte.PK_COD_ENTIDADE = '{$_REQUEST['entid']}' 
 AND NTE.nteenviofnde = 'E'
order by NTE.NTESEQUENCIAL;";
  
    }
    
//'<a href=\"pdeescola.php?modulo=meprincipal/meValidaEscolasPendentes&acao=A&entid='||et.entid||'&memid='||me.memid||'\" target=\"_blank\">'
    $listagem = new Simec_Listagem();
    $cabecalho = array(
        "Turma",
        "Nivel",
        "Situação",
        "Validar CPF/Turma");
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    ?>

    <?php if (false === $listagem->render()): ?>
        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>
        <?php
    endif;
}
//validar a turma pelo gestor estadual ou municipal
if ($_REQUEST['exec'] == 'validar') {
    $sqlNTE = "UPDATE eja.novaturmaeja
   SET nteenviofnde2p='V'
 WHERE nteid = {$_REQUEST['nteid']}";
    $db->executar($sqlNTE);
    echo "<script>alert('Operação realizada com sucesso!'); window.location = 'eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A&entid=" . $_REQUEST['entid'] . "&requisicao=listaTurmas';</script>";
}

//desvalidar a turma pelo gestor estadual ou municipal
if ($_REQUEST['exec'] == 'desvalidar') {
    $sqlNTE = "UPDATE eja.novaturmaeja
   SET nteenviofnde2p='C'
 WHERE nteid = {$_REQUEST['nteid']}";
    $db->executar($sqlNTE);
    echo "<script>alert('Operação realizada com sucesso!'); window.location = 'eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A&entid=" . $_REQUEST['entid'] . "&requisicao=listaTurmas';</script>";
}

//1º listagem dos Municipios relacionados ao gestor estadual
if (in_array(PERFIL_EJA_GESTOR_ESTADUAL, $perfis) && !$_REQUEST['requisicao'] && !$_REQUEST['exec']) {
    $sqlEst = "select 
            '<a href=\"eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A&requisicao=listaEscolas&muncod='||mun.muncod||'\">'||mundescricao||'</a>' as descricao, 
            ur.estuf from territorios.municipio mun 
	inner join eja.usuarioresponsabilidade ur ON mun.estuf = ur.estuf AND rpustatus = 'A' where   usucpf = '{$_SESSION['usucpf']}' order by mundescricao";
//        ver($sqlEst,d);
    $listagem = new Simec_Listagem();
    $cabecalho = array(
        "Município",
        "UF");
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sqlEst);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    ?>

    <?php if (false === $listagem->render()): ?>
        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>
        <?php
    endif;
}
//2º listagem das escolas relacionados ao municipio escolhido pelo gestor estadual
if (in_array(PERFIL_EJA_GESTOR_ESTADUAL, $perfis) && ($_REQUEST['requisicao'] == 'listaEscolas' && !$_REQUEST['exec'])) {
    ?>
    <div class="page-header">
        <h4 id="forms">Confirmação de CPF</h4>   
    </div>
    <br><?php
    $sqlEscolas = "SELECT
							
								 '<a href=\"eja.php?modulo=principal/confirmarcpf/confirmar_cpf&acao=A&requisicao=listaTurmas&entid='||ent.entid||'\">'||ent.entnome||'</a>' as nome,
								 ende.endlog || ' - '|| ende.endbai as endereco,
								 est.estuf as uf,
								 mun.mundescricao as mun 
								
								FROM
								 entidade.entidade ent 
								 INNER JOIN entidade.funcaoentidade fe ON fe.entid = ent.entid
								 --INNER JOIN entidade.entidadedetalhe ed ON ed.entid = ent.entid -- AND ed.entpdeescola = 't'
								 INNER JOIN entidade.endereco ende ON ende.entid = ent.entid
								 INNER JOIN territorios.municipio mun ON mun.muncod = ende.muncod
								 INNER JOIN territorios.estado est ON est.estuf = mun.estuf
								 LEFT JOIN eja.usuarioresponsabilidade ur ON ur.muncod = ende.muncod AND
								 	rpustatus = 'A'	
					where mun.muncod = '{$_REQUEST['muncod']}' AND fe.funid in (3,4) AND ent.entstatus='A'
								ORDER BY
								 ent.entnome";

    $listagem = new Simec_Listagem();
    $cabecalho = array(
        "Escola",
        "Endereço",
        "UF",
        "Município");
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sqlEscolas);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

    if (false === $listagem->render()):
        ?>
        <div class="alert alert-info col-md-4 col-md-offset-4 text-center">
            Nenhum registro encontrado
        </div>
        <?php
    endif;
}
?>
