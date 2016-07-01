<?php

/**
 * Class Ted_Model_Migrations
 */
class Ted_Model_Migrations extends Modelo
{

    const SIS_ID_ELABREV = 2;

    const SIS_ID_TED = 194;

    /**
     * @var array
     */
    protected $_oldPerfis = array(
        23 => 'Super Usuário',
        54 => 'UO/Equipe Técnica',
        852 => 'Gestor Orçamentário do Proponente',
        864 => 'Representante Legal do Proponente',
        859 => 'Gabinete Secretaria/Autarquia',
        866 => 'Coordenador da Secretaria/Autarquia',
        860 => 'Diretoria da Secretaria/Autarquia',
        865 => 'Representante Legal do Concedente',
        863 => 'Gestor Orçamentário do Concedente',
        871 => 'Área técnica do FNDE',
        1052 => 'Diretoria FNDE',
        862 => 'UG Repassadora',
        388 => 'Auditor Interno',          //1371
        57 => 'UO / Consulta Orçamento',  //1372
        851 => 'Diretor Administrativo',  //1373
    );

    /**
     * @var array
     */
    protected $_roleMaps = array(
        23 => 1233, //super usuario
        //1274 => 'Consulta',
        852 => 1262, //=> 'Gestor Orçamentário do Proponente',
        859 => 1264, //=> 'Gabinete Secretaria/Autarquia',
        866 => 1265, //=> 'Coordenador da Secretaria/Autarquia',
        860 => 1266, //=> 'Diretoria da Secretaria/Autarquia',
        864 => 1263, //=> 'Representante Legal do Proponente',
        865 => 1267, //=> 'Representante Legal do Concedente',
        863 => 1268, //=> 'Gestor Orçamentário do Concedente',
        871 => 1269, //=> 'Área técnica do FNDE',
        1052 => 1270, //=> 'Diretoria FNDE',
        54 => 1271, //=> 'UO/Equipe Técnica',
        862 => 1273, //=> 'UG Repassadora',
        388 => 1371, //=>'Auditor Interno'
        57  => 1372, //=>'UO / Consulta Orçamento'
        851 => 1373  //=>'Diretor Administrativo'
    );

    /**
     *
     */
    public function migrationUserResponsability()
    {
        $strSQL = "
            select
              usucpf, rpustatus, rpudata_inc, pflcod, unicod, prsano, ungcod, cooid, dircod
            from elabrev.usuarioresponsabilidade where rpustatus = 'A'
        ";

        $collections = $this->carregar($strSQL);
        if ($collections) {
            $i = 1;

            $strSQL = "
                insert into ted.usuarioresponsabilidade (rpuid, pflcod, usucpf, rpustatus, rpudata_inc, unicod, prsano, ungcod, cooid, dircod)
            <br/>";

            foreach ($collections as $row) {

                if (!array_key_exists($row['pflcod'], $this->_roleMaps))
                    continue;

                $cooid = ($row['cooid']) ? $row['cooid'] : 'null';
                $dircod = ($row['dircod']) ? $row['dircod'] : 'null';

                $strSQL .= "
                  values ($i, {$this->_roleMaps[$row['pflcod']]}, '{$row['usucpf']}', '{$row['rpustatus']}', '{$row['rpudata_inc']}',
                  '{$row['unicod']}', '{$row['prsano']}', '{$row['ungcod']}', $cooid, $dircod),<br/>";

                $i++;
            }

            echo substr($strSQL, 0, -1);
        }
    }

    /**
     *
     */
    public function migrationUserProfile()
    {
        $strSQL = "
            select * from seguranca.perfilusuario where pflcod in (23, 852, 864, 54, 859, 865, 863, 862, 860, 866, 1052, 871);
        ";

        $collections = $this->carregar($strSQL);
        if ($collections) {
            foreach ($collections as $row) {

                if (!array_key_exists($row['pflcod'], $this->_roleMaps))
                    continue;

                if ($this->pegaUm("select * from seguranca.perfilusuario where usucpf = '{$row['usucpf']}' and pflcod = {$this->_roleMaps[$row['pflcod']]}"))
                    continue;

                echo "insert into seguranca.perfilusuario (usucpf, pflcod)
                      values ('{$row['usucpf']}', {$this->_roleMaps[$row['pflcod']]});<br />";
            }
        }
    }

    /**
     *
     */
    public function migrationsUserSystem()
    {
        $strSQL = "
            select * from seguranca.usuario_sistema
            where sisid = ".self::SIS_ID_ELABREV." and pflcod in (23, 852, 864, 54, 859, 865, 863, 862, 860, 866, 1052, 871)
            and usucpf not in (
                select usucpf from seguranca.usuario_sistema where sisid = ".self::SIS_ID_TED."
            )
        ";

        $collections = $this->carregar($strSQL);
        if ($collections) {
            foreach ($collections as $row) {

                if (!array_key_exists($row['pflcod'], $this->_roleMaps))
                    continue;

                $row['susdataultacesso'] = ($row['susdataultacesso'])? $row['susdataultacesso'] : '2010-04-22 10:05:37';

                if ($this->pegaLinha("select * from seguranca.usuario_sistema where sisid = ".self::SIS_ID_TED." and usucpf = '{$row['usucpf']}' and pflcod = {$this->_roleMaps[$row['pflcod']]}")) {
                    continue;
                }

                echo "insert into seguranca.usuario_sistema(usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
                      values('{$row['usucpf']}', ".self::SIS_ID_TED.", '{$row['susstatus']}', {$this->_roleMaps[$row['pflcod']]}, '{$row['susdataultacesso']}', '{$row['suscod']}');<br />";
            }
        }
    }


    public function migrationsTprPerfil()
    {
        $strSQL = "select * from elabrev.tprperfil where pflcod in (23, 852, 864, 54, 859, 865, 863, 862, 860, 866, 1052, 871)";
        $collections = $this->carregar($strSQL);
        if ($collections) {

            $idMaps = array(
                4 => 1, //'sem associacao',
                6 => 2, //'programa',
                8 => 3, //'acao',
                9 => 4, //'unidade',
                1 => 5, //'unidade gestora',
                11 => 6, //'coordenacao',
                3 => 7, //'diretoria',
                2 => 8, //'unidade gestora direta',
            );

            $i = 1;
            foreach ($collections as $row) {
                if (!array_key_exists($row['pflcod'], $this->_roleMaps))
                    continue;

                echo "insert into ted.tprperfil (prfid, tprcod, pflcod) values ($i, {$idMaps[$row['tprcod']]}, {$this->_roleMaps[$row['pflcod']]});<br />";

                $i++;
            }
        }
    }

    public function migrationTipoResponsabilidade()
    {
        $strSQL = "
            select
                tprdsc, tprsnvisivelperfil, tprsigla, tprurl, tprtabela, tprcampo, tprcampodsc
            from elabrev.tiporesponsabilidade
        ";

        $collections = $this->carregar($strSQL);
        if ($collections) {
            $i = 1;
            foreach ($collections as $row) {

                echo "insert into ted.tiporesponsabilidade(tprcod,tprdsc,tprsnvisivelperfil,tprsigla,tprurl,tprtabela,tprcampo,tprcampodsc)
                      values($i,
                             '{$row['tprdsc']}',
                             '{$row['tprsnvisivelperfil']}',
                             '{$row['tprsigla']}',
                             '{$row['tprurl']}',
                             '{$row['tprtabela']}',
                             '{$row['tprcampo']}',
                             '{$row['tprcampodsc']}');<br />";

                $i++;
            }
        }

    }

    /**
     *
     */
    public function migrationsProfileWorkflow()
    {
        $strSQL = "
            select
                e.aedid, e.pflcod
            from
            workflow.estadodocumentoperfil e
            inner join (
                SELECT
                ae.aedid
                FROM workflow.acaoestadodoc ae
                    INNER JOIN workflow.estadodocumento ed ON (ed.esdid = ae.esdidorigem)
                WHERE
                    ed.tpdid = 97
                    AND aedstatus = 'A'
                ORDER BY esdordem ASC
            ) as v on (v.aedid = e.aedid)
        ";

        $collections = $this->carregar($strSQL);
        if ($collections) {
            $strInsert = "INSERT INTO workflow.estadodocumentoperfil(aedid, pflcod) values";
            foreach ($collections as $row) {
                if ($pflcod = $this->_roleMaps[$row['pflcod']]) {
                    $strInsert.= "
                        ({$row['aedid']}, {$pflcod}),<br/>
                    ";
                }
            }

            return $strInsert;
        }
    }

}