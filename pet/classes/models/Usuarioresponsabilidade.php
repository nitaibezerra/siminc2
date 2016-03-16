<?php

class Model_Usuarioresponsabilidade extends Abstract_Model
{
    protected $_schema = 'pet';
    protected $_name = 'usuarioresponsabilidade';
    public $entity = array();

    public function __construct($commit = true)
    {
        parent::__construct($commit);
        $this->db = new cls_banco();
        $this->entity['rpuid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'NO', 'maximum' => '', 'contraint' => 'pk');
        $this->entity['pflcod'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
        $this->entity['usucpf'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '11', 'contraint' => 'fk');
        $this->entity['rpustatus'] = array('value' => '', 'type' => 'character', 'is_null' => 'YES', 'maximum' => '1', 'contraint' => '');
        $this->entity['rpudata_inc'] = array('value' => '', 'type' => 'timestamp without time zone', 'is_null' => 'YES', 'maximum' => '', 'contraint' => '');
        $this->entity['iesid'] = array('value' => '', 'type' => 'integer', 'is_null' => 'YES', 'maximum' => '', 'contraint' => 'fk');
    }

    function listaDados()
    {
        $db = new cls_banco();

        $dados = $db->carregar("
          SELECT inst.iesid, inst.nome
                  FROM pet.institutoensinosuperior inst
                  WHERE inst.iesid NOT IN (SELECT iesid FROM pet.usuarioresponsabilidade WHERE rpustatus = 'A')
                  ORDER BY inst.nome");
        $count = count($dados);

        $div = "  <div class='list-group'> ";

        for ($i = 0; $i < $count; $i++) {
            $codigo = $dados[$i]["iesid"];
            $descricao = $dados[$i]["nome"];

            if (fmod($i, 2) == 0) {
                $cor = '#f4f4f4';
            } else {
                $cor = '#e0e0e0';
            }
            $div .= "
                  <a href='#' class='list-group-item' style='background-color: {$cor}; font-size: 11px;'>
                        <input type='radio' name='iesid' value='{$codigo}'>
                        $codigo - $descricao
                   </a>
            ";
        }
        $div .= " </div> ";
        echo $div;
    }

    function atribuirResponsabilidade($usucpf, $pflcod, $iesid)
    {
        $db = new cls_banco();
        $db->executar("UPDATE pet.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '{$usucpf}' AND pflcod = '{$pflcod}'");
        if ($iesid) {
            $dadosur = $db->carregar("SELECT * FROM pet.usuarioresponsabilidade WHERE usucpf = '{$usucpf}' AND pflcod = '{$pflcod}' AND iesid = '{$iesid}'");
            if ($dadosur) { // Se existir registro atualizar para ativo
                $db->carregar("UPDATE pet.usuarioresponsabilidade SET rpustatus = 'A', rpudata_inc= NOW() WHERE usucpf = '{$usucpf}' AND pflcod = '{$pflcod}' AND iesid = '{$iesid}'");
            } else { // Se não existir, inserir novo
                $db->executar("INSERT INTO pet.usuarioresponsabilidade(pflcod, usucpf, iesid, rpustatus, rpudata_inc) VALUES ('{$pflcod}', '{$usucpf}', '{$iesid}', 'A', NOW());");
            }
        }
        $db->commit();

        echo '<script>
			alert("Operação realizada com sucesso!");
			window.parent.opener.location.reload();
			self.close();
		  </script>';
    }

    public function getTipoResponsabilidadeByPerfil($pflcod)
    {
        $sql = "
            SELECT
                tr.* FROM pet.tprperfil p
            INNER JOIN pet.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
            WHERE tprsnvisivelperfil = TRUE AND p.pflcod = '%s'
            ORDER BY tr.tprdsc
        ";

        $query = sprintf($sql, $pflcod);
        return $this->db->carregar($query);
        $this->db->close();
    }

    public function getLista($rp, $usucpf, $pflcod)
    {
        $sqlRespUsuario = "";
        switch ($rp["tprsigla"]) {
            case "I":
                $sqlRespUsuario = "
                        SELECT  DISTINCT e.iesid AS codigo,
                                e.nome AS descricao,
                                ur.rpustatus AS status
                        FROM pet.usuarioresponsabilidade ur
                        INNER JOIN pet.institutoensinosuperior e ON e.iesid = ur.iesid
                        WHERE ur.usucpf = '%s' AND ur.pflcod = '%s' AND  ur.rpustatus='A'
                    ";
                break;
        }
        $query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
        return $this->db->carregar($query);
        $this->db->close();
    }

    public function getListaResposabilidade()
    {
        $sql = "
                SELECT  DISTINCT e.iesid, e.nome,
                        ur.rpustatus AS status
                FROM pet.usuarioresponsabilidade ur
                INNER JOIN pet.institutoensinosuperior e ON e.iesid = ur.iesid
                WHERE ur.usucpf = '%s' AND ur.pflcod IN (%s) AND  ur.rpustatus='A'
            ";

        $perfis = pegaPerfilGeral();
        $perfil = implode(',', $perfis);
        $query = vsprintf($sql, array($_SESSION['usucpf'], $perfil));
        $dados = $this->db->carregar($query);
        $this->db->close();

        if (is_array($dados)) {
            return $dados[0];
        }
        return $dados;
    }
}
