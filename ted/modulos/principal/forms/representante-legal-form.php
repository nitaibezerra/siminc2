<form class="form-horizontal"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form">

    <?= $this->element->ug; ?>
    <?= $this->element->substituto; ?>

    <div class="form-group">
        <div class="col-md-12">
            <table class="table table-condensed">
                <thead>
                    <tr>
                        <th colspan="4" class="text-center">Representante Legal Substituto</th>
                    </tr>
                    <tr>
                        <th><label for="cpf">CPF</labe></th>
                        <th><label for="nome">Nome</labe></th>
                        <th><label for="funcao">Função</labe></th>
                        <th><label for="email">E-mail</labe></th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td id="div_cpf">
                            <?= $this->element->rlid; ?>
                            <?= $this->element->cpf; ?>
                        </td>
                        <td id="div_nome">
                            <?= $this->element->nome; ?>
                        </td>
                        <td id="div_funcao">
                            <?= $this->element->funcao; ?>
                        </td>
                        <td id="div-email">
                            <?= $this->element->email; ?>
                        </td>
                    </tr>
                </tbody>
            </table>
        </div>
    </div>
</form>

<script type="text/javascript">
$(function(){
    $("#btn-save").on("click", function(e){
        e.preventDefault();

        var inputs = ['cpf', 'nome', 'email']
          , errors = false;

        $(".input-rl").parent().removeClass("has-error");

        for (var i=0; inputs.length>i; i++) {
            if (!$("#"+inputs[i]).val()) {
                var formGroup = $("#"+inputs[i]).parent();
                $(formGroup).addClass("has-error");
                errors = true;
            }
        }

        if (!validaCPF($("#cpf").val())) {
            $("#cpf").attr('placeholder', 'Digite um CPF válido').val("");
            $("#cpf").parent().addClass("has-error");
            errors = false;
        }

        if (errors) return false;

        $.ajax({
            url: location.href,
            type:'POST',
            data:{
                rlid:$("#rlid").val(),
                cpf:$("#cpf").val(),
                nome:$("#nome").val(),
                email:$("#email").val(),
                tcpid:$("#tcpid").val(),
                representantelegal:true
            },
            success:function(data) {
                //console.log(data);
                $('body').append(data);
            }
        });
    });
});

/**
 * Valida CPF front-end
 * @param cpf
 * @returns {boolean}
 */
function validaCPF(cpf) {
    var numeros, digitos, soma, i, resultado, digitos_iguais;
    digitos_iguais = 1;

    if (cpf.length < 11) {
        return false;
    }

    for (i = 0; i < cpf.length - 1; i++) {
        if (cpf.charAt(i) != cpf.charAt(i + 1)) {
            digitos_iguais = 0;
            break;
        }
    }

    if (!digitos_iguais) {
        numeros = cpf.substring(0,9);
        digitos = cpf.substring(9);
        soma = 0;

        for (i = 10; i > 1; i--) {
            soma += numeros.charAt(10 - i) * i;
        }

        resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;

        if (resultado != digitos.charAt(0)) {
            return false;
        }

        numeros = cpf.substring(0,10);
        soma = 0;

        for (i = 11; i > 1; i--) {
            soma += numeros.charAt(11 - i) * i;
        }

        resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;

        if (resultado != digitos.charAt(1))
            return false;

        return true;
    } else {
        return false;
    }
}
</script>